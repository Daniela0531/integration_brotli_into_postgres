To start:
./configure --without-icu --prefix=/Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/install
export PATH=$PATH:/Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/install/bin
make all
make install
mkdir pgdata
initdb -D /Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/pgdata
pg_ctl -D /Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/pgdata -l logfile start
psql -d postgres

To restart:
make clean
rm -rf install/
rm -rf pgdata/
rm -rf bin/
rm -rf include/
rm -rf lib/
rm -rf share/


test command:
CREATE TABLESPACE my_tablespace LOCATION '/Users/daniela/Desktop/my_tablespace' WITH (compression=true);


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
