To start:
./configure --without-icu --prefix=/Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/install
export PATH=$PATH:/Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/install/bin
make all -j8
make install
mkdir pgdata

--------------
./configure --without-icu --prefix=/Users/daniela/Desktop/research_work/final_qualifying_work/vanilla/postgresql/install
export PATH=$PATH:/Users/daniela/Desktop/research_work/final_qualifying_work/vanilla/postgresql/install/bin
make all -j8
make install
mkdir pgdata

initdb -D /Users/daniela/Desktop/research_work/final_qualifying_work/vanilla/postgresql/pgdata
--------------

initdb -D /Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/pgdata -l logfile

pg_ctl -D /Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/pgdata -l logfile start
psql -d postgres


"/Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/install/bin/postgres" --single -F -O -j -c search_path=pg_catalog -D /Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/pgdata -c exit_on_error=true -c log_checkpoints=false template1
ERROR:
postgres: could not access the server configuration file "/Users/daniela/Desktop/research_work/final_qualifying_work/brotli_into_postgres/pgdata/postgresql.conf": No such file or directory

PostgreSQL stand-alone backend 17devel
backend> 

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

lldb -c /cores/core.3906
bt

* thread #1, stop reason = ESR_EC_DABORT_EL0 (fault address: 0x0)
  * frame #0: 0x0000000104012a58 postgres`heap_compute_data_size + 256
    frame #1: 0x0000000104013c50 postgres`heap_form_tuple + 188
    frame #2: 0x00000001040c6e50 postgres`CreateConstraintEntry + 1348
    frame #3: 0x00000001040b4d64 postgres`AddRelationNotNullConstraints + 260
    frame #4: 0x000000010414f420 postgres`DefineRelation + 6424
    frame #5: 0x0000000104332e80 postgres`ProcessUtilitySlow + 400
    frame #6: 0x00000001043323e4 postgres`standard_ProcessUtility + 1524
    frame #7: 0x0000000104331ae0 postgres`PortalRunUtility + 136
    frame #8: 0x000000010433130c postgres`PortalRunMulti + 240
    frame #9: 0x0000000104330df0 postgres`PortalRun + 452
    frame #10: 0x0000000104330010 postgres`exec_simple_query + 1448
    frame #11: 0x000000010432d5dc postgres`PostgresMain + 1152
    frame #12: 0x0000000104329da4 postgres`BackendMain + 80
    frame #13: 0x000000010429f118 postgres`postmaster_child_launch + 192
    frame #14: 0x00000001042a3238 postgres`ServerLoop + 6784
    frame #15: 0x00000001042a0c44 postgres`PostmasterMain + 3436
    frame #16: 0x00000001041d041c postgres`main + 800
    frame #17: 0x00000001836120e0 dyld`start + 2360