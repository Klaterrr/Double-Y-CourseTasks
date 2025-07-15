WITH event_pairs AS (
    SELECT
        verdict,
        verdict_time,
        LEAD(verdict, 1) OVER (PARTITION BY campaign_id ORDER BY verdict_time) AS next_verdict,
        LEAD(verdict_time, 1) OVER (PARTITION BY campaign_id ORDER BY verdict_time) AS next_verdict_time
    FROM
        logs
),
wait_times AS (
    SELECT
        DATE(verdict_time) AS wait_start_date,
        (strftime('%s', next_verdict_time) - strftime('%s', verdict_time)) / 60.0 AS wait_minutes
    FROM
        event_pairs
    WHERE
        verdict = 'No' AND next_verdict = 'Yes'
)
SELECT
    wait_start_date AS field_date,
    ROUND(AVG(wait_minutes)) AS avg_wait_time
FROM
    wait_times
GROUP BY
    field_date
ORDER BY
    field_date ASC;