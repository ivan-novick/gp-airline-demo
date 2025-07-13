SELECT
    t.ticket_id,
    t.seat_number,
    t.price,
    t.booking_time,
    p.first_name || ' ' || p.last_name AS passenger_name,
    p.email,
    f.flight_number,
    f.departure_time,
    f.arrival_time,
    f.status,
    a1.name AS departure_airport,
    a2.name AS arrival_airport
FROM
    tickets t
JOIN
    passengers p ON t.passenger_id = p.passenger_id
JOIN
    flights f ON t.flight_id = f.flight_id
JOIN
    airports a1 ON f.departure_airport_id = a1.airport_id
JOIN
    airports a2 ON f.arrival_airport_id = a2.airport_id
ORDER BY
    t.ticket_id;

