# golang database 全局 - 三少GG - CSDN博客
2016年01月02日 22:24:36[三少GG](https://me.csdn.net/scut1135)阅读数：1153
重要的前提条件：
## type [DB](https://golang.org/src/database/sql/sql.go?s=5752:6728#L211)
type DB struct {
        // contains filtered or unexported fields
}
DB is a database handle representing a pool of zero or more underlying connections.**It's safe for concurrent use by multiple goroutines.**
The sql package creates and frees connections automatically; it also maintains a free pool of idle connections. If the database has a concept of per-connection state, such state can only be reliably observed within a transaction. Once DB.Begin is called, the
 returned Tx is bound to a single connection. Once Commit or Rollback is called on the transaction, that transaction's connection is returned to DB's idle connection pool. The pool size can be controlled with SetMaxIdleConns.
### func [Open](https://golang.org/src/database/sql/sql.go?s=13127:13184#L458)
func Open(driverName, dataSourceName [string](https://golang.org/pkg/builtin/#string)) (*[DB](https://golang.org/pkg/database/sql/#DB), [error](https://golang.org/pkg/builtin/#error))
Open opens a database specified by its database driver name and a driver-specific data source name, usually consisting of at least a database name and connection information.
Most users will open a database via a driver-specific connection helper function that returns a *DB. No database drivers are included in the Go standard library. See [https://golang.org/s/sqldrivers](https://golang.org/s/sqldrivers) for
 a list of third-party drivers.
Open may just validate its arguments without creating a connection to the database. To verify that the data source name is valid, call Ping.
**The returned DB is safe for concurrent use by multiple goroutines and maintains its own pool of idle connections. Thus, the Open function should be called just once. It is rarely necessary to close a DB.**
##################
Go-MySQL-Driver的作者的回复 http://stackoverflow.com/questions/17376207/how-to-share-mysql-connection-between-http-goroutines
The database/sql package manages the connection pooling automatically for you.
**`sql.Open(..)` returns a handle which represents a connection pool, not a single connection. The database/sql package automatically opens a new connection if all connections in the pool are busy.**
Applied to your code this means, that you just need to share the db-handle and use it in the HTTP handlers:
```
package main
import (
    "database/sql"
    "fmt"
    "github.com/gorilla/mux"
    _ "github.com/go-sql-driver/mysql"
    "log"
    "net/http"
)
var db *sql.DB // global variable to share it between main and the HTTP handler
func main() {
    fmt.Println("starting up")
    var err error
    db, err = sql.Open("mysql", "root@unix(/tmp/mysql.sock)/mydb") // this does not really open a new connection
    if err != nil {
        log.Fatalf("Error on initializing database connection: %s", err.Error())
    }
    db.SetMaxIdleConns(100)
    err = db.Ping() // This DOES open a connection if necessary. This makes sure the database is accessible
    if err != nil {
        log.Fatalf("Error on opening database connection: %s", err.Error())
    }
    r := mux.NewRouter()
    r.HandleFunc("/", HomeHandler)
    http.Handle("/", r)
    http.ListenAndServe(":8080", nil)
}
func HomeHandler(w http.ResponseWriter, r *http.Request) {
    var msg string
    err := db.QueryRow("SELECT msg FROM hello WHERE page=?", "home").Scan(&msg)
    if err != nil {
        fmt.Fprintf(w, "Database Error!")
    } else {
        fmt.Fprintf(w, msg)
    }
}
```
```
```
其余分析： http://studygolang.com/articles/3022
