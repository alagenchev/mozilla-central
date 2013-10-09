#include <sqlite3.h>

namespace alagenchev {
    enum DomainType {
        eFinancialDomain = 0x00,
        eOtherDomain = 0x01
    };

    class ArktikFox {
        public:
            static int GetDomainType(nsCOMPtr<nsIURI> aURI, DomainType *type) {
                sqlite3 *db;
                int result;
                nsAutoCString spec;
                aURI->GetSpec(spec);

                result = sqlite3_open("/home/alagenchev/mozilla-central/database/websites.sqlite", &db);
                if( result ){
                    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
                    sqlite3_close(db);
                    return 1;
                }
                nsAutoCString query;
                query.Assign("select count(1) as COUNT from websites where domain='");
                query.Append(spec);
                query.Append("';");

                sqlite3_stmt *selectStmt;
                sqlite3_prepare(db, query.get(), query.Length() + 1, &selectStmt, NULL);

                int count = 0;
                int step_result = sqlite3_step (selectStmt);
                if (step_result == SQLITE_ROW) {
                    count = sqlite3_column_int(selectStmt, 0);

                }
                else {

                    sqlite3_close(db);
                    return 0;
                }


                if(count > 0) {
                    *type = eFinancialDomain;
                }
                else {
                    *type = eOtherDomain;
                }

                sqlite3_close(db);
                return 0;
            }
    };
}
