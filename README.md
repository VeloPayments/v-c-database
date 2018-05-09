Velo C Database Library
=======================

The Velo C Database Library provides an interface that abstracts database I/O as
a simple key-value map.  The library provides the ability to maintain
connections to multiple key-value maps and to synchronize transactions between
them.  Under the covers, this is translated as calls to LevelDB, Berkeley DB,
SQLite, Postgres, Oracle, etc.
