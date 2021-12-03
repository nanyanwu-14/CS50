import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get username, symbols for currently owned stocks, prices of these stocks, and current cash value
    username = (db.execute("SELECT username FROM users WHERE id = ?", session["user_id"]))[0]["username"]
    symbols_dict = (db.execute("SELECT DISTINCT(symbol) FROM shares WHERE username = ?", username))
    prices_dict = (db.execute("SELECT DISTINCT(price) FROM shares WHERE username = ?", username))
    cash = round((db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"]))[0]["cash"], 2)

    # Initialize empty lists for values to be displayed
    symbols = []
    prices = []
    total_shares = []
    names = []
    subtotals = []

    # Add symbols and prices from table to lists
    for i in range(len(symbols_dict)):
        symbols.append(symbols_dict[i]["symbol"])
        prices.append(prices_dict[i]["price"])

    # Add company names and shares for each company to lists
    for symbol in symbols:
        names.append((lookup(symbol))["name"])
        total_shares.append(db.execute("SELECT SUM(stocks) FROM shares WHERE username = ? AND symbol = ?",
                                       username, symbol)[0]["SUM(stocks)"])

    # Get current total value of stocks
    for i in range(len(prices)):
        subtotals.append(round(prices[i] * total_shares[i], 2))

    # Remove rows where number of shares is 0
    i = 0
    while i < len(total_shares):
        if total_shares[i] == 0:
            del total_shares[i]
            del symbols[i]
            del names[i]
            del subtotals[i]
        else:
            i += 1

    # Sum cash and stock values to get total
    total = round(sum(subtotals) + cash, 2)

    return render_template("index.html", total=total, length=len(symbols), symbols=symbols, names=names, shares=total_shares, prices=prices, subtotals=subtotals, cash=cash)


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def cash():
    """Add cash to account"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get symbol and share amount from form
        cash_str = request.form.get("cash")

        # Ensure symbol and shares are entered
        if not cash_str:
            return apology("enter amount of money to add")

        # Ensure that cash value is a positive integer:
        cash = int(cash_str)
        if cash <= 0:
            return apology("money amount must be a positive integer")

        # Get Current Balance
        balance = (db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"]))[0]["cash"]

        # Update user balance
        new_balance = balance + cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_balance, session["user_id"])

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("addcash.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get symbol and share amount from form
        symbol = request.form.get("symbol")
        shares_str = request.form.get("shares")

        # Ensure symbol and shares are entered
        if not symbol:
            return apology("no symbol entered")
        if not shares_str:
            return apology("enter number of shares to sell")

        # Get current price of shares
        quote = lookup(symbol)

        # Ensure symbol is valid
        if not quote:
            return apology("invalid symbol")

        # Ensure the number of shares does not contain letters and is positive
        if not shares_str.isnumeric() or shares_str == '0':
            return apology("number of shares must be a positive integer")

        # Get important values
        shares = int(shares_str)
        price = quote["price"]
        balance = (db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"]))[0]["cash"]

        # Ensure that user has enough money to purchase stocks
        if (price * shares) > balance:
            return apology("current balance is insufficient to purchase")

        # Get username using session id
        username = (db.execute("SELECT username FROM users WHERE id = ?", session["user_id"]))[0]["username"]

        # Update number of shares
        db.execute("INSERT INTO shares (username, symbol, stocks, price) VALUES (?,?,?,?)", username, symbol, shares, price)

        # Update user balance
        new_balance = balance - (price * shares)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_balance, session["user_id"])

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get username, symbols for purchased stocks, prices of these stocks, and timestamps for purchase
    username = (db.execute("SELECT username FROM users WHERE id = ?", session["user_id"]))[0]["username"]
    symbols_dict = db.execute("SELECT symbol FROM shares WHERE username = ?", username)
    prices_dict = db.execute("SELECT price FROM shares WHERE username = ?", username)
    stocks_dict = db.execute("SELECT stocks FROM shares WHERE username = ?", username)
    timestamps_dict = db.execute("SELECT timestamp FROM shares WHERE username = ?", username)

    # Initialize empty lists for values to be displayed
    symbols = []
    prices = []
    stocks = []
    timestamps = []

    # Add company symbols, stock prices, stock numbers, and timestamps to lists
    for i in range(len(symbols_dict)):
        symbols.append(symbols_dict[i]["symbol"])
        prices.append(prices_dict[i]["price"])
        stocks.append(stocks_dict[i]["stocks"])
        timestamps.append(timestamps_dict[i]["timestamp"])

    return render_template("history.html", length=len(symbols), symbols=symbols, prices=prices, shares=stocks, timestamps=timestamps)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        symbol = request.form.get("symbol")

        # Check that symbol is entered
        if not symbol:
            return apology("symbol not entered")

        # Check that symbol is valid
        quote = lookup(symbol)
        if not quote:
            return apology("symbol was not found")

        # Return the quote through the quoted.html page
        return render_template("quoted.html", name=quote["name"], price=quote["price"])

    else:
        # If accessing /quote via get, show the quote.html page
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register User"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Check that username does not exist
        if len(rows) != 0:
            return apology("this username already exists")

        # Check that username is entered
        if not username:
            return apology("no username entered")

        # Check that password is entered
        elif not password:
            return apology("no password entered")

        # Check the password is re-entered
        elif not confirmation:
            return apology("password was not confirmed")

        # Check that password and re-entered passwords match
        elif password != confirmation:
            return apology("passwords do not match")

        # Store users username and password hash in database
        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, generate_password_hash(password))
        return redirect("/")

    else:
        # If accessing /register via get, show the registration page
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get symbol and share amount from form
        symbol = request.form.get("symbol")
        shares_str = request.form.get("shares")

        # Ensure symbol and shares are entered
        if not symbol:
            return apology("no symbol selected")
        if not shares_str:
            return apology("enter number of shares to sell")

        # Ensure the number of shares does not contain letters and is positive
        if not shares_str.isnumeric() or shares_str == '0':
            return apology("number of shares must be a positive integer")
        shares = int(shares_str)

        # Get current price of shares
        price = (lookup(symbol))["price"]

        # Get username using session id
        username = (db.execute("SELECT username FROM users WHERE id = ?", session["user_id"]))[0]["username"]

        # Get total shares in company
        total_shares = db.execute("SELECT SUM(stocks) FROM shares WHERE username = ? AND symbol = ?",
                                  username, symbol)[0]["SUM(stocks)"]

        # Ensure that the user has enough shares to sell
        if total_shares - shares < 0:
            return apology("you do not have enough shares to sell")

        # Update number of shares
        db.execute("INSERT INTO shares (username, symbol, stocks, price) VALUES (?,?,?,?)", username, symbol, -shares, price)

        # Update user balance
        balance = (db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"]))[0]["cash"]

        new_balance = balance + (price * shares)

        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_balance, session["user_id"])

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Get user's username
        username = (db.execute("SELECT username FROM users WHERE id = ?", session["user_id"]))[0]["username"]

        # Get the symbols associated with the username
        symbols = (db.execute("SELECT DISTINCT(symbol) FROM shares WHERE username = ?", username))

        # Get total chares
        total_shares = []
        for symbol in symbols:
            total_shares.append(db.execute("SELECT SUM(stocks) FROM shares WHERE username = ? AND symbol = ?",
                                           username, symbol["symbol"])[0]["SUM(stocks)"])

        # Remove symbols where number of shares is 0
        i = 0
        while i < len(total_shares):
            if total_shares[i] == 0:
                del total_shares[i]
                del symbols[i]
            else:
                i += 1

        return render_template("sell.html", symbols=symbols)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
