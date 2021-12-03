SELECT name FROM people WHERE name != "Kevin Bacon" AND id IN
    (SELECT person_id FROM stars WHERE movie_id IN
        (SELECT stars.movie_id FROM people
        JOIN stars ON people.id = stars.person_id
        WHERE name = "Kevin Bacon" AND birth = 1958));