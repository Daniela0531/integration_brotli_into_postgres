To start:
./configure --without-icu --prefix=/Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/install
export PATH=$PATH:/Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/install/bin
make all -j8
make install
mkdir pgdata

initdb -D /Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/pgdata -l logfile

pg_ctl -D /Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/pgdata -l logfile start
psql -d postgres


"/Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/install/bin/postgres" --single -F -O -j -c search_path=pg_catalog -D /Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/pgdata -c exit_on_error=true -c log_checkpoints=false template1
ERROR:
postgres: could not access the server configuration file "/Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/pgdata/postgresql.conf": No such file or directory

To restart:
make clean
rm -rf install/
rm -rf pgdata/
rm -rf bin/
rm -rf include/
rm -rf lib/
rm -rf share/


log_min_messages=debug5

test command:
CREATE TABLESPACE my_tablespace LOCATION '/Users/daniela/Desktop/my_tablespace';

CREATE TABLESPACE my_tablespace LOCATION '/Users/daniela/Desktop/my_tablespace' WITH (compression=true);

CREATE TABLE cinemas (id serial, name text, location text) TABLESPACE my_tablespace;
INSERT INTO cinemas DEFAULT VALUES;
INSERT INTO cinemas (name, location) VALUES('name', 'location');

INSERT INTO cinemas (name, location) SELECT md5(random()::text), md5(random()::text) || '@gmail.com' FROM generate_series(1, 100);

CHECKPOINT;
DROP TABLESPACE my_tablespace;

PostgreSQL Database Management System
=====================================

This directory contains the source code distribution of the PostgreSQL
database management system.

PostgreSQL is an advanced object-relational database management system
that supports an extended subset of the SQL standard, including
transactions, foreign keys, subqueries, triggers, user-defined types
and functions.  This distribution also contains C language bindings.

Copyright and license information can be found in the file COPYRIGHT.

General documentation about this version of PostgreSQL can be found at
<https://www.postgresql.org/docs/devel/>.  In particular, information
about building PostgreSQL from the source code can be found at
<https://www.postgresql.org/docs/devel/installation.html>.

The latest version of this software, and related software, may be
obtained at <https://www.postgresql.org/download/>.  For more information
look at our web site located at <https://www.postgresql.org/>.
