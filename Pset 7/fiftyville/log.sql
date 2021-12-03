-- Keep a log of any SQL queries you execute as you solve the mystery. --

-- Searching for the description of the crime to get some additional info
--------------------------------------------------------------------------
SELECT description FROM crime_scene_reports WHERE month = 7 AND day = 28 AND street = "Chamberlin Street";
-- Info returned:
-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. Interviews were conducted
-- today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.

-- Checking the courthouse interviews
--------------------------------------
SELECT name, transcript FROM interviews WHERE month = 7 AND day = 28 AND year = 2020 AND transcript LIKE "%courthouse%";
-- Info returned:
-- Ruth: Within 10 minutes of the theft saw thief get into car in the courthouse lot
-- Eugene: Saw the thief in the morning before arriving at the courthouse by the ATM on Fifer Street
-- Raymond: Thief called someone for less than a minute as they left courthouse. Thief was planning to take earliest flight out. Person on the phone bought the ticket.

-- Checking courthouse security logs
-------------------------------------
SELECT license_plate FROM courthouse_security_logs WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 05 AND minute <= 25 AND activity = "exit";

-- Checking call logs for caller number
----------------------------------------
SELECT caller FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60;

-- Checking flights
--------------------
SELECT passport_number FROM flights
JOIN passengers ON flights.id = passengers.flight_id
JOIN airports ON flights.origin_airport_id = airports.id
WHERE airports.city LIKE "Fiftyville" AND flights.day = 29 AND flights.year = 2020 AND flights.month = 7 AND flights.hour <= 12;

-- Using current info to narrow down suspects
----------------------------------------------
SELECT name FROM people
WHERE phone_number IN 
    (SELECT caller FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60)
AND passport_number IN
(
    SELECT passport_number FROM flights
    JOIN passengers ON flights.id = passengers.flight_id
    JOIN airports ON flights.origin_airport_id = airports.id
    WHERE airports.city LIKE "Fiftyville" AND flights.day = 29 AND flights.year = 2020 AND flights.month = 7 AND flights.hour <= 12
)
AND license_plate IN 
    (SELECT license_plate FROM courthouse_security_logs WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 05 AND minute <= 25 AND activity = "exit");
-- Info Returned:
-- Roger, Evelyn, Ernest

-- Checking bank transactions to narrow suspect names
------------------------------------------------------
SELECT name FROM bank_accounts
JOIN people ON bank_accounts.person_id = people.id
WHERE bank_accounts.account_number IN
    (SELECT account_number FROM atm_transactions WHERE year = 2020 AND month = 7 AND day = 28 AND atm_location = "Fifer Street");
-- Info Returned:
-- Ernest, Robert, Russell, Roy, Bobby, Elizabeth, Danielle, Madison, Victoria

-------------------------------
-- The thief must be Ernest! --
-------------------------------

-- Finding accomplice
----------------------
SELECT name FROM people WHERE phone_number =
    (SELECT receiver FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60 AND caller =
        (SELECT phone_number FROM people WHERE name = "Ernest"));

---------------------------------
-- The accomplice is Berthold! --
---------------------------------

-- Finding escape destination
------------------------------
SELECT city FROM airports WHERE id =
    (SELECT destination_airport_id FROM flights WHERE id =
        (SELECT flight_id FROM passengers WHERE passport_number =
            (SELECT passport_number FROM people WHERE name = "Ernest")));

--------------------------
-- Escaping to London!  --
--------------------------