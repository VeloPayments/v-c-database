Velo C Database Library
=======================

The Velo C Database Library provides an interface that abstracts database I/O as
a simple key-value map.  The library provides the ability to maintain
connections to multiple key-value maps and to synchronize transactions between
them.  Under the covers, this is translated as calls to LevelDB, Berkeley DB,
SQLite, Postgres, Oracle, etc.

Because this interface provides a common interface between database backends, it
does not take advantage of all of the features for each particular database
backend.  Instead, it provides a key-value mapping view that includes the
ability to associate secondary indices to the data.  Since data is serialized to
the database, the secondary index provides a method to extract the secondary
key.  In a key-value database like Berkeley DB, this results in a secondary
index data store that associates the primary key and secondary key.  In a SQL
database, this results in an index and a secondary key.

Datastore Interface
-------------------

The `vcdb_datastore_t` interface provides a way to define a key-value data
store, to define its configuration, to associate methods for serializing data,
and to extract the key from the structured data type.

Secondary Index Interface
-------------------------

The `vcdb_index_t` interface provides a way to define a secondary index for a
given `vcdb_datastore_t` datastore.

Database Builder
----------------

The initial interface for creating the database is the `vcdb_builder_t`
interface.  This interface allows data stores and indices to be associated with
the database.  This interface provides three methods:
`vcdb_builder_database_create`, `vcdb_builder_database_open`, and
`vcdb_builder_database_destroy`.  These methods create, open, and destroy the
associated database accordingly.

Database interface
------------------

The `vcdb_database_t` interface provides a mechanism for translating actions on
the database to specific commands.  The handle interface is used to maintain
state with the underlying database.  It also provides a means to query
datastores and secondary indices outside of a transaction (i.e. in read-only
mode).

Transaction interface
---------------------

The `vcdb_transaction_t` interface is the primary means to upgrade a database
handle to a mutable data structure.  Once a transaction is started, new records
can be inserted, old records can be deleted, and the transaction can be
committed or abandoned.

The transaction interface is also required to manage upgrades and recovery of
the database.  In these particular cases, special transactions are started which
are used to perform the upgrades or recoveries independently of any other
operations.
