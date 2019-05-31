## RocksServer Protocol

### Get
Get value by key

Request:
```
GET /get?key HTTP/1.1
```
Reply:

```
<value_len>\n
<value>
```
If the key does not exist in the database, the reply will be:
```
-1\n
```

### Multi get
Get values by keys

Request:
```
GET /mget?key1&key2&key3 HTTP/1.1
```
Reply:

```
<key1>\n
<value1_len>\n
<value1>\n
<key2>\n
<value2_len>\n
<value2>\n
<key3>\n
<value3_len>\n
<value3>\n
```
If the keyN does not exist, then `<valueN_len> = -1`, `<valueN>` -- is empty


### Set
Set value by key

Request:
```
POST /set HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key>\n
<value_len>\n
<value>
```
Reply:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.


### Multi set
Set values by keys

Request:
```
POST /mset HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key1>\n
<value1_len>\n
<value1>
<key2>\n
<value2_len>\n
<value2>
<key3>\n
<value3_len>\n
<value3>
```
Reply:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.

### Delete key
Delete key from DB

Request:
```
POST /del HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key>
```
Reply:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.


### Multi delete keys
Delete keys from DB

Request:
```
POST /mdel HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key1>\n
<key2>\n
<key3>
```
Reply:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.

### Key exist?
Check key existing

Request:
```
GET /exist?key HTTP/1.1
```
Reply:

```
OK
<value_len>\n
<value>
```
if the key exists and is able to retrieve its value.

Reply:
```
OK
-1\n
```
if the key exists but was not able to retrieve its value.

Reply:
```
FAIL
```
if the key does not exist in the database.

### Imcrement
Imcrement value by key

Request:
```
POST /incr HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key>
```
to increase the value on 1.

Or request:
```
POST /incr HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key>\n
<value>
```
to increase the value on `<value>`.

Reply:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.

### Get all keys and values

Request:
```
POST /tail
```

Reply: the same as [multi get](https://github.com/valmat/RocksServer/blob/master/protocol.md#multi-get)


### Get keys and values by key-prefix

Request:
```
GET /prefit?<prefix>
```

Reply: the same as [multi get](https://github.com/valmat/RocksServer/blob/master/protocol.md#multi-get)


### Get keys and values by seek key-prefix

Request:
```
GET /seekprev?<key-prefix-start>[&<starts-with>]
```
```
GET /seekprev-range?<key-prefix-start>&<key-prefix-end>[&<starts-with>]
```
```
GET /seeknext?<key-prefix-start>[&<starts-with>]
```
```
GET /seeknext-range?<key-prefix-start>&<key-prefix-end>[&<starts-with>]
```

`seekprev` and `seekprev-range` seeks for first key less then `<key-prefix-start>`.

`seeknext` and `seeknext-range` seeks for first key equal or more then `<key-prefix-start>`.


`seekprev-range` and `seeknext-range` iterate while key less or equal `<key-prefix-end>`

If optional parameter `<starts-with>` given iterate, while key starts with prefix `<starts-with>`


Reply: the same as [multi get](https://github.com/valmat/RocksServer/blob/master/protocol.md#multi-get)


### Backup
Backup database

Request:
```
POST /backup
```

Reply:
```
OK
```
If successful.
Or reply:
```
<Error message>
```
on fail.


### Stats
Usage statistics

Request:
```
GET /stats
```

Reply: stats output
