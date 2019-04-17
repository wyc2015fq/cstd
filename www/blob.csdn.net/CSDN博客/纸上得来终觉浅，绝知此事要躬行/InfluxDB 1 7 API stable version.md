# InfluxDB 1.7 API stable version - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年11月13日 08:52:40[boonya](https://me.csdn.net/boonya)阅读数：525








[https://docs.influxdata.com/influxdb/v1.7/tools/api/](https://docs.influxdata.com/influxdb/v1.7/tools/api/)

The InfluxDB HTTP API provides a simple way interact with the database. It uses HTTP response codes, HTTP authentication, JWT Tokens, and basic authentication, and responses are returned in JSON.

The following sections assume your InfluxDB instance is running on `localhost` port `8086` and HTTPS is not enabled. Those settings [are configurable](https://docs.influxdata.com/influxdb/v1.7/administration/config/#http-endpoints-settings).

## [InfluxDB HTTP endpoints](https://docs.influxdata.com/influxdb/v1.7/tools/api/#influxdb-http-endpoints)
|Endpoint|Description|
|----|----|
|[/debug/pprof](https://docs.influxdata.com/influxdb/v1.7/tools/api/#debug-pprof-http-endpoint)|Use `/debug/pprof` to generate profiles for troubleshooting.|
|[/debug/requests](https://docs.influxdata.com/influxdb/v1.7/tools/api/#debug-requests-http-endpoint)|Use `/debug/requests/` to track HTTP client requests to the `/write` and `/query` endpoints.|
|[/debug/vars](https://docs.influxdata.com/influxdb/v1.7/tools/api/#debug-vars-http-endpoint)|Use `/debug/vars` to collect statistics|
|[/ping](https://docs.influxdata.com/influxdb/v1.7/tools/api/#ping-http-endpoint)|Use `/ping` to check the status of your InfluxDB instance and your version of InfluxDB.|
|[/query](https://docs.influxdata.com/influxdb/v1.7/tools/api/#query-http-endpoint)|Use `/query` to query data and manage databases, retention policies, and users.|
|[/write](https://docs.influxdata.com/influxdb/v1.7/tools/api/#write-http-endpoint)|Use `/write` to write data to a pre-existing database.|

## [`/debug/pprof` HTTP endpoint](https://docs.influxdata.com/influxdb/v1.7/tools/api/#debug-pprof-http-endpoint)

InfluxDB supports the Go [`net/http/pprof`](https://golang.org/pkg/net/http/pprof/) HTTP endpoints, which are useful for troubleshooting. The `pprof` package serves runtime profiling data in the format expected by the *pprof* visualization tool.

[Definition](https://docs.influxdata.com/influxdb/v1.7/tools/api/#definition)

```
curl http://localhost:8086/debug/pprof/
```

The `/debug/pprof/` endpoint generates an HTML page with a list of built-in Go profiles and hyperlinks for each.
|Profile|Description|
|----|----|
|block|Stack traces that led to blocking on synchronization primitives.|
|goroutine|Stack traces of all current goroutines.|
|heap|Sampling of stack traces for heap allocations.|
|mutex|Stack traces of holders of contended mutexes.|
|threadcreate|Stack traces that led to the creation of new OS threads.|

To access one of the the `/debug/pprof/` profiles listed above, use the following cURL request, substituting `<profile>` with the name of the profile. The resulting profile is output to a file specified in `<path\to\output-file>`.

```
curl -o <path/to/output-file>  http://localhost:8086/debug/pprof/<profile>
```

In the following example, the cURL command outputs the resulting heap profile to a file:

```
curl -o <path/to/output-file> http://localhost:/8086/debug/pprof/heap
```

You can also use the [Go `pprof` interactive tool](https://github.com/google/pprof) to access the InfluxDB `/debug/pprof/` profiles. For example, to look at the heap profile of a InfluxDB instance using this tool, you would use a command like this:

```
go tool pprof http://localhost:8086/debug/pprof/heap
```

For more information about the Go `/net/http/pprof` package and the interactive *pprof* analysis and visualization tool, see:
- [Package pprof (`net/http/pprof`)](https://golang.org/pkg/net/http/pprof/)
- [`pprof` analysis and visualization tool](https://github.com/google/pprof)
- [Profiling Go programs](https://blog.golang.org/profiling-go-programs)
- [Diagnostics - Profiling](https://golang.org/doc/diagnostics.html#profiling)

[`/debug/pprof/all` HTTP endpoint](https://docs.influxdata.com/influxdb/v1.7/tools/api/#debug-pprof-all-http-endpoint)

The `/debug/pprof/all` endpoint is a custom `/debug/pprof` profile intended primarily for use by InfluxData support. This endpoint generates a `profile.tar.gz` archive containing text files with the standard Go profiling information and additional debugging data. An optional CPU profile is generated when using the `cpu=true` option (default is `false`).

To create a `profile.tar.gz` archive, use the following cURL command to generate a `profile.tar.gz` file for sharing with InfluxData support.

```
curl -o profiles.tar.gz "http://localhost:8086/debug/pprof/all?cpu=true"
```

> 
**Note:** When the `cpu=true` option is included, a CPU profile is generated for 30+ seconds. If you’re concerned about running a CPU profile (which only has a small, temporary impact on performance), then you can set `?cpu=false` or omit `?cpu=true` altogether.


As the following example shows, the cURL output includes “Time Spent,” the time elapsed (in seconds). After 30 seconds of data has been collected, the results are output to a file.

```
➜  ~ curl -o profiles.tar.gz "http://localhost:8086/debug/pprof/all?cpu=true"
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100  237k    0  237k    0     0   8025      0 --:--:--  0:00:30 --:--:-- 79588
```

## [`/debug/requests` HTTP endpoint](https://docs.influxdata.com/influxdb/v1.7/tools/api/#debug-requests-http-endpoint)

Use this endpoint to track HTTP client requests to the `/write` and `/query` endpoints. The `/debug/requests` endpoint returns the number of writes and queries to InfluxDB per username and IP address.

### [Definition](https://docs.influxdata.com/influxdb/v1.7/tools/api/#definition-1)

```
curl http://localhost:8086/debug/requests
```

### [Query string parameters](https://docs.influxdata.com/influxdb/v1.7/tools/api/#query-string-parameters)
|Query String Parameter|Optional/Required|Definition|
|----|----|----|
|seconds=<integer>|Optional|Sets the duration (in seconds) over which the client collects information. The default duration is ten seconds.|

[Examples](https://docs.influxdata.com/influxdb/v1.7/tools/api/#examples)

Track requests over a ten-second interval

```
$ curl http://localhost:8086/debug/requests

{
"user1:123.45.678.91": {"writes":1,"queries":0},
}
```

The response shows that, over the past ten seconds, the `user1` user sent one request to the `/write` endpoint and no requests to the `/query` endpoint from the `123.45.678.91` IP address.

Track requests over a one-minute interval

```
$ curl http://localhost:8086/debug/requests?seconds=60

{
"user1:123.45.678.91": {"writes":3,"queries":0},
"user1:000.0.0.0": {"writes":0,"queries":16},
"user2:xx.xx.xxx.xxx": {"writes":4,"queries":0}
}
```

The response shows that, over the past minute, `user1` sent three requests to the `/write` endpoint from `123.45.678.91`, `user1` sent 16 requests to the `/query` endpoint from `000.0.0.0`, and `user2` sent four requests to the `/write` endpoint from `xx.xx.xxx.xxx`.

## [`/debug/vars` HTTP endpoint](https://docs.influxdata.com/influxdb/v1.7/tools/api/#debug-vars-http-endpoint)

InfluxDB exposes statistics and information about its runtime through the `/debug/vars` endpoint, which can be accessed using the following cURL command:

```
curl http://localhost:8086/debug/vars
```

Server statistics and information are displayed in JSON format.

> 
**Note:** The [InfluxDB input plugin](https://github.com/influxdata/telegraf/tree/release-1.7/plugins/inputs/influxdb) is available to collect metrics (using the `/debug/vars` endpoint) from specified Kapacitor instances. For a list of the measurements and fields, see the [InfluxDB input plugin README](https://github.com/influxdata/telegraf/tree/release-1.7/plugins/inputs/influxdb).


## [`/ping` HTTP endpoint](https://docs.influxdata.com/influxdb/v1.7/tools/api/#ping-http-endpoint)

The ping endpoint accepts both `GET` and `HEAD` HTTP requests. Use this endpoint to check the status of your InfluxDB instance and your version of InfluxDB.

### [Definition](https://docs.influxdata.com/influxdb/v1.7/tools/api/#definition-2)

```
GET http://localhost:8086/ping
```

```
HEAD http://localhost:8086/ping
```

### [`verbose` option](https://docs.influxdata.com/influxdb/v1.7/tools/api/#verbose-option)

By default, the `/ping` HTTP endpoint returns a simple HTTP 204 status response to let the client know that the server is running. Default value is `false`. When `verbose` option is set to `true` (`/ping?verbose=true`), an HTTP 200 status is returned. The `verbose=true` option is required for health checks in the Google Kubernetes Cloud.

### [Example](https://docs.influxdata.com/influxdb/v1.7/tools/api/#example)

You can use the `/ping` endpoint to find the build and version of an InfluxDB instance. The `X-Influxdb-Build` header field displays the InfluxDB build type, either `OSS` (open source) or `ENT` (Enterprise). The `X-Influxdb-Version` header field displays the InfluxDB version.

```
~ curl -sl -I http://localhost:8086/ping

HTTP/1.1 204 No Content
Content-Type: application/json
Request-Id: 9c353b0e-aadc-11e8-8023-000000000000
X-Influxdb-Build: OSS
X-Influxdb-Version: v1.7.0
X-Request-Id: 9c353b0e-aadc-11e8-8023-000000000000
Date: Tue, 05 Nov 2018 16:08:32 GMT
```

### [Status Codes and Responses](https://docs.influxdata.com/influxdb/v1.7/tools/api/#status-codes-and-responses)

The response body is empty.
|HTTP Status Code|Description|
|----|----|
|204|Success! Your InfluxDB instance is up and running.|

## [`/query` HTTP endpoint](https://docs.influxdata.com/influxdb/v1.7/tools/api/#query-http-endpoint)

The `/query` endpoint accepts `GET` and `POST` HTTP requests. Use this endpoint to query data and manage databases, retention policies, and users.

### [Definition](https://docs.influxdata.com/influxdb/v1.7/tools/api/#definition-3)

```
GET http://localhost:8086/query
```

```
POST http://localhost:8086/query
```

### [Verb usage](https://docs.influxdata.com/influxdb/v1.7/tools/api/#verb-usage)
|Verb|Query Type|
|----|----|
|GET|Use for all queries that start with:[`SELECT`](https://docs.influxdata.com/influxdb/v1.7/query_language/spec/#select)*[`SHOW`](https://docs.influxdata.com/influxdb/v1.7/query_language/spec/#show-continuous-queries)|
|POST|Use for all queries that start with:[`ALTER`](https://docs.influxdata.com/influxdb/v1.7/query_language/spec/#alter-retention-policy)[`CREATE`](https://docs.influxdata.com/influxdb/v1.7/query_language/spec/#create-continuous-query)[`DELETE`](https://docs.influxdata.com/influxdb/v1.7/query_language/spec/#delete)[`DROP`](https://docs.influxdata.com/influxdb/v1.7/query_language/spec/#drop-continuous-query)[`GRANT`](https://docs.influxdata.com/influxdb/v1.7/query_language/spec/#grant)[`KILL`](https://docs.influxdata.com/influxdb/v1.7/query_language/spec/#kill-query)[`REVOKE`](https://docs.influxdata.com/influxdb/v1.7/query_language/spec/#revoke)|

* The only exceptions are `SELECT` queries that include an [`INTO` clause](https://docs.influxdata.com/influxdb/v1.7/query_language/data_exploration/#the-into-clause). Those `SELECT` queries require a `POST` request.

[Examples](https://docs.influxdata.com/influxdb/v1.7/tools/api/#examples-1)

Query data with a `SELECT` statement

```
$ curl -G 'http://localhost:8086/query?db=mydb' --data-urlencode 'q=SELECT * FROM "mymeas"'

{"results":[{"statement_id":0,"series":[{"name":"mymeas","columns":["time","myfield","mytag1","mytag2"],"values":[["2017-03-01T00:16:18Z",33.1,null,null],["2017-03-01T00:17:18Z",12.4,"12","14"]]}]}]}
```

The `mymeas`[measurement](https://docs.influxdata.com/influxdb/v1.7/concepts/glossary/#measurement) has two points. The first point has the [timestamp](https://docs.influxdata.com/influxdb/v1.7/concepts/glossary/#timestamp)`2017-03-01T00:16:18Z`, a `myfield` value of `33.1`, and no tag values for the `mytag1` and `mytag2`[tag keys](https://docs.influxdata.com/influxdb/v1.7/concepts/glossary/#tag-key). The second point has the timestamp `2017-03-01T00:17:18Z`, a `myfield` value of `12.4`, a `mytag1` value of `12`, and a `mytag2` value of `14`.

The same query in InfluxDB’s [Command Line Interface](https://docs.influxdata.com/influxdb/v1.7/tools/shell/) (CLI) returns the following table:

```
name: mymeas
time                  myfield  mytag1  mytag2
----                  -------  ------  ------
2017-03-01T00:16:18Z  33.1
2017-03-01T00:17:18Z  12.4     12      14
```

Query data with a `SELECT` statement and an `INTO` clause

```
$ curl -XPOST 'http://localhost:8086/query?db=mydb' --data-urlencode 'q=SELECT * INTO "newmeas" FROM "mymeas"'

{"results":[{"statement_id":0,"series":[{"name":"result","columns":["time","written"],"values":[["1970-01-01T00:00:00Z",2]]}]}]}
```

`SELECT` queries that include and [`INTO` clause](https://docs.influxdata.com/influxdb/v1.7/query_language/data_exploration/#the-into-clause) require a `POST` request.

The response shows that InfluxDB writes two points to the `newmeas`[measurement](https://docs.influxdata.com/influxdb/v1.7/concepts/glossary/#measurement). Note that the system uses epoch 0 (`1970-01-01T00:00:00Z`) as a [null timestamp equivalent](https://docs.influxdata.com/influxdb/v1.7/troubleshooting/frequently-asked-questions/#why-does-my-query-return-epoch-0-as-the-timestamp).

Create a database

```
$ curl -XPOST 'http://localhost:8086/query' --data-urlencode 'q=CREATE DATABASE "mydb"'

{"results":[{"statement_id":0}]}
```

A successful [`CREATE DATABASE` query](https://docs.influxdata.com/influxdb/v1.7/query_language/database_management/#create-database) returns no additional information.

### [Query string parameters](https://docs.influxdata.com/influxdb/v1.7/tools/api/#query-string-parameters-1)
|Query String Parameter|Optional/Required|Definition|
|----|----|----|
|chunked=[true | <number_of_points>]|Optional|Returns points in streamed batches instead of in a single response. If set to `true`, InfluxDB chunks responses by series or by every 10,000 points, whichever occurs first. If set to a specific value, InfluxDB chunks responses by series or by that number of points.*|
|db=<database_name>|Required for database-dependent queries (most [`SELECT`](https://docs.influxdata.com/influxdb/v1.7/query_language/spec/#select) queries and [`SHOW`](https://docs.influxdata.com/influxdb/v1.7/query_language/spec/#show-continuous-queries) queries require this parameter).|Sets the target [database](https://docs.influxdata.com/influxdb/v1.7/concepts/glossary/#database) for the query.|
|epoch=[ns,u,µ,ms,s,m,h]|Optional|Returns epoch timestamps with the specified precision. By default, InfluxDB returns timestamps in RFC3339 format with nanosecond precision. Both `u` and `µ` indicate microseconds.|
|p=<password>|Optional if you haven’t [enabled authentication](https://docs.influxdata.com/influxdb/v1.7/administration/authentication_and_authorization/#set-up-authentication). Required if you’ve enabled authentication.**|Sets the password for authentication if you’ve enabled authentication. Use with the query string parameter `u`.|
|pretty=true|Optional|Enables pretty-printed JSON output. While this is useful for debugging it is not recommended for production use as it consumes unnecessary network bandwidth.|
|q=<query>|Required|InfluxQL string to execute. See also [Request Body](https://docs.influxdata.com/influxdb/v1.7/tools/api/#request-body).|
|u=<username>|Optional if you haven’t [enabled authentication](https://docs.influxdata.com/influxdb/v1.7/administration/authentication_and_authorization/#set-up-authentication). Required if you’ve enabled authentication.*|Sets the username for authentication if you’ve enabled authentication. The user must have read access to the database. Use with the query string parameter `p`.|

* InfluxDB does not truncate the number of rows returned for requests without the `chunked` parameter. That behavior is configurable; see the [`max-row-limit`](https://docs.influxdata.com/influxdb/v1.7/administration/config/#max-row-limit-0) configuration option for more information.

** The HTTP API also supports basic authentication. Use basic authentication if you’ve [enabled authentication](https://docs.influxdata.com/influxdb/v1.7/administration/authentication_and_authorization/#set-up-authentication) and aren’t using the query string parameters `u` and `p`. See below for an [example](https://docs.influxdata.com/influxdb/v1.7/tools/api/#create-a-database-using-basic-authentication) of basic authentication.

[Examples](https://docs.influxdata.com/influxdb/v1.7/tools/api/#examples-2)

Query data with a `SELECT` statement and return pretty-printed JSON

```
$ curl -G 'http://localhost:8086/query?db=mydb&pretty=true' --data-urlencode 'q=SELECT * FROM "mymeas"'

{
    "results": [
        {
            "statement_id": 0,
            "series": [
                {
                    "name": "mymeas",
                    "columns": [
                        "time",
                        "myfield",
                        "mytag1",
                        "mytag2"
                    ],
                    "values": [
                        [
                            "2017-03-01T00:16:18Z",
                            33.1,
                            null,
                            null
                        ],
                        [
                            "2017-03-01T00:17:18Z",
                            12.4,
                            "12",
                            "14"
                        ]
                    ]
                }
            ]
        }
    ]
}
```

Query data with a `SELECT` statement and return second precision epoch timestamps

```
$ curl -G 'http://localhost:8086/query?db=mydb&epoch=s' --data-urlencode 'q=SELECT * FROM "mymeas"'

{"results":[{"statement_id":0,"series":[{"name":"mymeas","columns":["time","myfield","mytag1","mytag2"],"values":[[1488327378,33.1,null,null],[1488327438,12.4,"12","14"]]}]}]}
```

Create a database using HTTP authentication

Valid credentials:

```
$ curl -XPOST 'http://localhost:8086/query?u=myusername&p=mypassword' --data-urlencode 'q=CREATE DATABASE "mydb"'

{"results":[{"statement_id":0}]}
```

A successful [`CREATE DATABASE` query](https://docs.influxdata.com/influxdb/v1.7/query_language/database_management/#create-database) returns no additional information.

Invalid credentials:

```
$ curl -XPOST 'http://localhost:8086/query?u=myusername&p=notmypassword' --data-urlencode 'q=CREATE DATABASE "mydb"'

{"error":"authorization failed"}
```

Create a database using basic authentication

The following example uses valid credentials.

```
$ curl -XPOST -u myusername:mypassword 'http://localhost:8086/query' --data-urlencode 'q=CREATE DATABASE "mydb"'

{"results":[{"statement_id":0}]}
```

A successful [`CREATE DATABASE` query](https://docs.influxdata.com/influxdb/v1.7/query_language/database_management/#create-database) returns no additional information.

The following example uses invalid credentials.

```
$ curl -XPOST -u myusername:notmypassword 'http://localhost:8086/query' --data-urlencode 'q=CREATE DATABASE "mydb"'

{"error":"authorization failed"}
```

### [Request body](https://docs.influxdata.com/influxdb/v1.7/tools/api/#request-body)

```
--data-urlencode "q=<InfluxQL query>"
```

All queries must be URL encoded and follow [InfluxQL](https://docs.influxdata.com/influxdb/v1.7/query_language/) syntax. Our example shows the `--data-urlencode` parameter from `curl`, which we use in all examples on this page.

[Options](https://docs.influxdata.com/influxdb/v1.7/tools/api/#options)

Request multiple queries

Delimit multiple queries with a semicolon `;`.

Submit queries from a file

The API supports submitting queries from a file using a multipart `POST` request. The queries in the file must be separated a semicolon (`;`).

Syntax:

```
curl -F "q=@<path_to_file>" -F "async=true" http://localhost:8086/query
```

Request query results in CSV format

Syntax:

```
curl -H "Accept: application/csv" -G 'http://localhost:8086/query [...]
```

Note that when the request includes `-H "Accept: application/csv"`, the system returns timestamps in `epoch` format, not RFC3339 format.

Bind parameters

The API supports binding parameters to particular field values or tag values in the [`WHERE` clause](https://docs.influxdata.com/influxdb/v1.7/query_language/data_exploration/#the-where-clause). Use the syntax `$<placeholder_key>` as a placeholder in the query, and URL encode the map of placeholder keys to placeholder values in the request body:

Query syntax:

```
--data-urlencode 'q= SELECT [...] WHERE [ <field_key> | <tag_key> ] = $<placeholder_key>'
```

Map syntax:

```
--data-urlencode 'params={"<placeholder_key>":[ <placeholder_float_field_value> | <placeholder_integer_field_value> | "<placeholder_string_field_value>" | <placeholder_boolean_field_value> | "<placeholder_tag_value>" ]}'
```

Delimit multiple placeholder key-value pairs with comma `,`.

[Examples](https://docs.influxdata.com/influxdb/v1.7/tools/api/#examples-3)

Send multiple queries

```
$ curl -G 'http://localhost:8086/query?db=mydb&epoch=s' --data-urlencode 'q=SELECT * FROM "mymeas";SELECT mean("myfield") FROM "mymeas"'

{"results":[{"statement_id":0,"series":[{"name":"mymeas","columns":["time","myfield","mytag1","mytag2"],"values":[[1488327378,33.1,null,null],[1488327438,12.4,"12","14"]]}]},{"statement_id":1,"series":[{"name":"mymeas","columns":["time","mean"],"values":[[0,22.75]]}]}]}
```

The request includes two queries: `SELECT * FROM "mymeas"` and `SELECT mean("myfield") FROM "mymeas"'`. In the results, the system assigns a statement identifier to each query return. The first query’s result has a `statement_id` of `0` and the second query’s result has a `statement_id` of `1`.

Request query results in CSV format

```
$ curl -H "Accept: application/csv" -G 'http://localhost:8086/query?db=mydb' --data-urlencode 'q=SELECT * FROM "mymeas"'

name,tags,time,myfield,mytag1,mytag2
mymeas,,1488327378000000000,33.1,mytag1,mytag2
mymeas,,1488327438000000000,12.4,12,14
```

The first point has no [tag values](https://docs.influxdata.com/influxdb/v1.7/concepts/glossary/#tag-value) for the `mytag1` and `mytag2`[tag keys](https://docs.influxdata.com/influxdb/v1.7/concepts/glossary/#tag-key).

Submit queries from a file

```
$ curl -F "q=@queries.txt" -F "async=true" 'http://localhost:8086/query'
```

A sample of the queries in `queries.txt`:

```
CREATE DATABASE mydb;
CREATE RETENTION POLICY four_weeks ON mydb DURATION 4w REPLICATION 1;
```

Bind a parameter in the `WHERE` clause to specific tag value

```
$ curl -G 'http://localhost:8086/query?db=mydb' --data-urlencode 'q=SELECT * FROM "mymeas" WHERE "mytag1" = $tag_value' --data-urlencode 'params={"tag_value":"12"}'

{"results":[{"statement_id":0,"series":[{"name":"mymeas","columns":["time","myfield","mytag1","mytag2"],"values":[["2017-03-01T00:17:18Z",12.4,"12","14"]]}]}]}
```

The request maps `$tag_value` to `12`. InfluxDB stores [tag values](https://docs.influxdata.com/influxdb/v1.7/concepts/glossary/#tag-value) as strings they and must be double quoted in the request.

Bind a parameter in the `WHERE` clause to a numerical field value

```
$ curl -G 'http://localhost:8086/query?db=mydb' --data-urlencode 'q=SELECT * FROM "mymeas" WHERE "myfield" > $field_value' --data-urlencode 'params={"field_value":30}'

{"results":[{"statement_id":0,"series":[{"name":"mymeas","columns":["time","myfield","mytag1","mytag2"],"values":[["2017-03-01T00:16:18Z",33.1,null,null]]}]}]}
```

The request maps `$field_value` to `30`. The value `30` does not require double quotes because `myfield` stores numerical [field values](https://docs.influxdata.com/influxdb/v1.7/concepts/glossary/#field-value).

Bind two parameters in the `WHERE` clause to a specific tag value and numerical field value

```
$ curl -G 'http://localhost:8086/query?db=mydb' --data-urlencode 'q=SELECT * FROM "mymeas" WHERE "mytag1" = $tag_value AND  "myfield" < $field_value' --data-urlencode 'params={"tag_value":"12","field_value":30}'

{"results":[{"statement_id":0,"series":[{"name":"mymeas","columns":["time","myfield","mytag1","mytag2"],"values":[["2017-03-01T00:17:18Z",12.4,"12","14"]]}]}]}
```

The request maps `$tag_value` to `12` and `$field_value` to `30`.

### [Status codes and responses](https://docs.influxdata.com/influxdb/v1.7/tools/api/#status-codes-and-responses-1)

Responses are returned in JSON. Include the query string parameter `pretty=true` to enable pretty-print JSON.

[Summary table](https://docs.influxdata.com/influxdb/v1.7/tools/api/#summary-table)
|HTTP status code|Description|
|----|----|
|200 OK|Success! The returned JSON offers further information.|
|400 Bad Request|Unacceptable request. Can occur with a syntactically incorrect query. The returned JSON offers further information.|
|401 Unauthorized|Unacceptable request. Can occur with invalid authentication credentials.|

[Examples](https://docs.influxdata.com/influxdb/v1.7/tools/api/#examples-4)

A successful request that returns data

```
$ curl -i -G 'http://localhost:8086/query?db=mydb' --data-urlencode 'q=SELECT * FROM "mymeas"'

HTTP/1.1 200 OK
Connection: close
Content-Type: application/json
Request-Id: [...]
X-Influxdb-Version: 1.4.x
Date: Wed, 08 Nov 2017 19:22:54 GMT
Transfer-Encoding: chunked

{"results":[{"statement_id":0,"series":[{"name":"mymeas","columns":["time","myfield","mytag1","mytag2"],"values":[["2017-03-01T00:16:18Z",33.1,null,null],["2017-03-01T00:17:18Z",12.4,"12","14"]]}]}]}
```

A successful request that returns an error

```
$ curl -i -G 'http://localhost:8086/query?db=mydb1' --data-urlencode 'q=SELECT * FROM "mymeas"'

HTTP/1.1 200 OK
Connection: close
Content-Type: application/json
Request-Id: [...]
X-Influxdb-Version: 1.4.x
Date: Wed, 08 Nov 2017 19:23:48 GMT
Transfer-Encoding: chunked

{"results":[{"statement_id":0,"error":"database not found: mydb1"}]}
```

An incorrectly formatted query

```
$ curl -i -G 'http://localhost:8086/query?db=mydb' --data-urlencode 'q=SELECT *'

HTTP/1.1 400 Bad Request
Content-Type: application/json
Request-Id: [...]
X-Influxdb-Version: 1.4.x
Date: Wed, 08 Nov 2017 19:24:25 GMT
Content-Length: 76

{"error":"error parsing query: found EOF, expected FROM at line 1, char 9"}
```

Query data with invalid authentication credentials

```
$ curl -i  -XPOST 'http://localhost:8086/query?u=myusername&p=notmypassword' --data-urlencode 'q=CREATE DATABASE "mydb"'

HTTP/1.1 401 Unauthorized
Content-Type: application/json
Request-Id: [...]
Www-Authenticate: Basic realm="InfluxDB"
X-Influxdb-Version: 1.4.x
Date: Wed, 08 Nov 2017 19:11:26 GMT
Content-Length: 33

{"error":"authorization failed"}
```

## [`/write` HTTP endpoint](https://docs.influxdata.com/influxdb/v1.7/tools/api/#write-http-endpoint)

The `/write` endpoint accepts `POST` HTTP requests. Use this endpoint to write data to a pre-existing database.

### [Definition](https://docs.influxdata.com/influxdb/v1.7/tools/api/#definition-4)

```
POST http://localhost:8086/write
```

### [Query string parameters](https://docs.influxdata.com/influxdb/v1.7/tools/api/#query-string-parameters-2)
|Query String Parameter|Optional/Required|Description|
|----|----|----|
|consistency=[any,one,quorum,all]|Optional, available with [InfluxDB Enterprise clusters](https://docs.influxdata.com/enterprise_influxdb/v1.6/) only.|Sets the write consistency for the point. InfluxDB assumes that the write consistency is `one` if you do not specify `consistency`. See the [InfluxDB Enterprise documentation](https://docs.influxdata.com/enterprise_influxdb/v1.6/concepts/clustering#write-consistency) for detailed descriptions of each consistency option.|
|db=<database>|Required|Sets the target [database](https://docs.influxdata.com/influxdb/v1.7/concepts/glossary/#database) for the write.|
|p=<password>|Optional if you haven’t [enabled authentication](https://docs.influxdata.com/influxdb/v1.7/administration/authentication_and_authorization/#set-up-authentication). Required if you’ve enabled authentication.*|Sets the password for authentication if you’ve enabled authentication. Use with the query string parameter `u`.|
|precision=[ns,u,ms,s,m,h]|Optional|Sets the precision for the supplied Unix time values. InfluxDB assumes that timestamps are in nanoseconds if you do not specify `precision`.**|
|rp=<retention_policy_name>|Optional|Sets the target [retention policy](https://docs.influxdata.com/influxdb/v1.7/concepts/glossary/#retention-policy-rp) for the write. InfluxDB writes to the `DEFAULT` retention policy if you do not specify a retention policy.|
|u=<username>|Optional if you haven’t [enabled authentication](https://docs.influxdata.com/influxdb/v1.7/administration/authentication_and_authorization/#set-up-authentication). Required if you’ve enabled authentication.*|Sets the username for authentication if you’ve enabled authentication. The user must have write access to the database. Use with the query string parameter `p`.|

* The HTTP API also supports basic authentication. Use basic authentication if you’ve [enabled authentication](https://docs.influxdata.com/influxdb/v1.7/administration/authentication_and_authorization/#set-up-authentication) and aren’t using the query string parameters `u` and `p`. See below for an [example](https://docs.influxdata.com/influxdb/v1.7/tools/api/#write-a-point-to-the-database-mydb-using-basic-authentication) of basic authentication.

** We recommend using the least precise precision possible as this can result in significant improvements in compression.

[Examples](https://docs.influxdata.com/influxdb/v1.7/tools/api/#examples-5)

Write a point to the database `mydb` with a timestamp in seconds

```
$ curl -i -XPOST "http://localhost:8086/write?db=mydb&precision=s" --data-binary 'mymeas,mytag=1 myfield=90 1463683075'

HTTP/1.1 204 No Content
Content-Type: application/json
Request-Id: [...]
X-Influxdb-Version: 1.4.x
Date: Wed, 08 Nov 2017 17:33:23 GMT
```

Write a point to the database `mydb` and the retention policy `myrp`

```
$ curl -i -XPOST "http://localhost:8086/write?db=mydb&rp=myrp" --data-binary 'mymeas,mytag=1 myfield=90'

HTTP/1.1 204 No Content
Content-Type: application/json
Request-Id: [...]
X-Influxdb-Version: 1.4.x
Date: Wed, 08 Nov 2017 17:34:31 GMT
```

Write a point to the database `mydb` using HTTP authentication

Valid credentials:

```
$ curl -i -XPOST "http://localhost:8086/write?db=mydb&u=myusername&p=mypassword" --data-binary 'mymeas,mytag=1 myfield=91'

HTTP/1.1 204 No Content
Content-Type: application/json
Request-Id: [...]
X-Influxdb-Version: 1.4.x
Date: Wed, 08 Nov 2017 17:34:56 GMT
```

Invalid credentials:

```
$ curl -i -XPOST "http://localhost:8086/write?db=mydb&u=myusername&p=notmypassword" --data-binary 'mymeas,mytag=1 myfield=91'

HTTP/1.1 401 Unauthorized
Content-Type: application/json
Request-Id: [...]
Www-Authenticate: Basic realm="InfluxDB"
X-Influxdb-Version: 1.4.x
Date: Wed, 08 Nov 2017 17:40:30 GMT
Content-Length: 33

{"error":"authorization failed"}
```

Write a point to the database `mydb` using basic authentication

Valid credentials:

```
$ curl -i -XPOST -u myusername:mypassword "http://localhost:8086/write?db=mydb" --data-binary 'mymeas,mytag=1 myfield=91'

HTTP/1.1 204 No Content
Content-Type: application/json
Request-Id: [...]
X-Influxdb-Version: 1.4.x
Date: Wed, 08 Nov 2017 17:36:40 GMT
```

Invalid credentials:

```
$ curl -i -XPOST -u myusername:notmypassword "http://localhost:8086/write?db=mydb" --data-binary 'mymeas,mytag=1 myfield=91'

HTTP/1.1 401 Unauthorized
Content-Type: application/json
Request-Id: [...]
Www-Authenticate: Basic realm="InfluxDB"
X-Influxdb-Version: 1.4.x
Date: Wed, 08 Nov 2017 17:46:40 GMT
Content-Length: 33

{"error":"authorization failed"}
```

### [Request body](https://docs.influxdata.com/influxdb/v1.7/tools/api/#request-body-1)

```
--data-binary '<Data in Line Protocol format>'
```

All data must be binary encoded and in the [Line Protocol](https://docs.influxdata.com/influxdb/v1.7/concepts/glossary/#line-protocol) format. Our example shows the `--data-binary` parameter from curl, which we will use in all examples on this page. Using any encoding method other than `--data-binary` will likely lead to issues; `-d`, `--data-urlencode`, and `--data-ascii` may strip out newlines or introduce new, unintended formatting.

Options:
- Write several points to the database with one request by separating each point by a new line.
- 
Write points from a file with the `@` flag. The file should contain a batch of points in the Line Protocol format. Individual points must be on their own line and separated by newline characters (`\n`). Files containing carriage returns will cause parser errors.

We recommend writing points in batches of 5,000 to 10,000 points. Smaller batches, and more HTTP requests, will result in sub-optimal performance.


[Examples](https://docs.influxdata.com/influxdb/v1.7/tools/api/#examples-6)

Example 1: Write a point to the database `mydb` with a nanosecond timestamp



```
$ curl -i -XPOST "http://localhost:8086/write?db=mydb" --data-binary 'mymeas,mytag=1 myfield=90 1463683075000000000'

HTTP/1.1 204 No Content
Content-Type: application/json
Request-Id: [...]
X-Influxdb-Version: 1.4.x
Date: Wed, 08 Nov 2017 18:02:57 GMT
```

Write a point to the database `mydb` with the local server’s nanosecond timestamp

```
$ curl -i -XPOST "http://localhost:8086/write?db=mydb" --data-binary 'mymeas,mytag=1 myfield=90'

HTTP/1.1 204 No Content
Content-Type: application/json
Request-Id: [...]
X-Influxdb-Version: 1.4.x
Date: Wed, 08 Nov 2017 18:03:44 GMT
```

Write several points to the database `mydb` by separating points with a new line

```
$ curl -i -XPOST "http://localhost:8086/write?db=mydb" --data-binary 'mymeas,mytag=3 myfield=89
mymeas,mytag=2 myfield=34 1463689152000000000'

HTTP/1.1 204 No Content
Content-Type: application/json
Request-Id: [...]
X-Influxdb-Version: 1.4.x
Date: Wed, 08 Nov 2017 18:04:02 GMT
```

Write several points to the database `mydb` from the file `data.txt`

```
$ curl -i -XPOST "http://localhost:8086/write?db=mydb" --data-binary @data.txt

HTTP/1.1 204 No Content
Content-Type: application/json
Request-Id: [...]
X-Influxdb-Version: 1.4.x
Date: Wed, 08 Nov 2017 18:08:11 GMT
```

A sample of the data in `data.txt`:

```
mymeas,mytag1=1 value=21 1463689680000000000
mymeas,mytag1=1 value=34 1463689690000000000
mymeas,mytag2=8 value=78 1463689700000000000
mymeas,mytag3=9 value=89 1463689710000000000
```

### [Status codes and responses](https://docs.influxdata.com/influxdb/v1.7/tools/api/#status-codes-and-responses-2)

In general, status codes of the form `2xx` indicate success, `4xx` indicate that InfluxDB could not understand the request, and `5xx` indicate that the system is overloaded or significantly impaired. Errors are returned in JSON.

[Summary table](https://docs.influxdata.com/influxdb/v1.7/tools/api/#summary-table-1)
|HTTP status code|Description|
|----|----|
|204 No Content|Success!|
|400 Bad Request|Unacceptable request. Can occur with a Line Protocol syntax error or if a user attempts to write values to a field that previously accepted a different value type. The returned JSON offers further information.|
|401 Unauthorized|Unacceptable request. Can occur with invalid authentication credentials.|
|404 Not Found|Unacceptable request. Can occur if a user attempts to write to a database that does not exist. The returned JSON offers further information.|
|500 Internal Server Error|The system is overloaded or significantly impaired. Can occur if a user attempts to write to a retention policy that does not exist. The returned JSON offers further information.|

[Examples](https://docs.influxdata.com/influxdb/v1.7/tools/api/#examples-7)

A successful write

```
HTTP/1.1 204 No Content
```

Write a point with an incorrect timestamp

```
HTTP/1.1 400 Bad Request
[...]
{"error":"unable to parse 'mymeas,mytag=1 myfield=91 abc123': bad timestamp"}
```

Write an integer to a field that previously accepted a float

```
HTTP/1.1 400 Bad Request
[...]
{"error":"field type conflict: input field \"myfield\" on measurement \"mymeas\" is type int64, already exists as type float"}
```

Write a point with invalid authentication credentials

```
HTTP/1.1 401 Unauthorized
[...]
{"error":"authorization failed"}
```

Write a point to a database that doesn’t exist

```
HTTP/1.1 404 Not Found
[...]
{"error":"database not found: \"mydb1\""}
```

Write a point to a retention policy that doesn’t exist

```
HTTP/1.1 500 Internal Server Error
[...]
{"error":"retention policy not found: myrp"}
```





