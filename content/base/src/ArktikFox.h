#ifndef alagenchev_fox_
#define alagenchev_fox_
#include <sqlite3.h>
#include <mutex>
#include <mozIStorageConnection.h>
#include "mozilla/storage.h"
#include "nsAppDirectoryServiceDefs.h"
#include <unistd.h>

class mozIStorageService;

namespace alagenchev {

    enum DomainType {
        eFinancialDomain = 0x00,
        eOtherDomain = 0x01
    };

    static std::mutex mtx;

    class ArktikFox {
        public:
            static nsresult IsThirdPartyContent(nsCOMPtr<nsIURI> aRequestingLocation,
                    nsCOMPtr<nsIURI> aContentLocation, bool &aIsThirdPartyContent)
            {
                nsAutoCString host;
                aRequestingLocation->GetHost(host);

                nsCOMPtr<nsIFile> dbFile;
                nsresult rv = getDBFile(getter_AddRefs(dbFile));
                NS_ENSURE_SUCCESS(rv, rv);

                nsCOMPtr<mozIStorageService> dbService =
                    do_GetService("@mozilla.org/storage/service;1", &rv);
                NS_ENSURE_SUCCESS(rv, rv);

                nsCOMPtr<mozIStorageConnection> dbConn;
                rv = dbService->OpenDatabase(dbFile, getter_AddRefs(dbConn));

                if(NS_FAILED(rv))
                {
                    fprintf(stderr, "ARKTIK FOX FAILED 1 \n");
                }
                NS_ENSURE_SUCCESS(rv, rv);
                nsAutoCString query;
                query.Assign("select id from websites where domain like '%");
                query.Append(host);
                query.Append("%';");

                nsCOMPtr<mozIStorageStatement> statement;
                rv = dbConn->CreateStatement(query, getter_AddRefs(statement));
                NS_ENSURE_SUCCESS(rv, rv);

                PRInt32 site_id = -1;
                bool hasMoreData;
                if (NS_SUCCEEDED(statement->ExecuteStep(&hasMoreData)) && hasMoreData) {
                    rv = statement->GetInt32(0, &site_id);
                    NS_ENSURE_SUCCESS(rv, rv);
                }

                statement->Reset();

                query.Assign("SELECT * FROM not_third_party where owner ='");
                query.AppendInt(site_id, 10);
                query.Append("';");


                rv = dbConn->CreateStatement(query, getter_AddRefs(statement));
                nsAutoString domain;
                nsAutoCString contentHost;
                aContentLocation->GetHost(contentHost);

                while(NS_SUCCEEDED(statement->ExecuteStep(&hasMoreData)) && hasMoreData) {
                    rv = statement->GetString(1, domain);
                    NS_ENSURE_SUCCESS(rv, rv);
                    nsAutoCString temp;
                    temp.Assign(NS_ConvertUTF16toUTF8(domain).get());
                    if(contentHost.Find(temp.get(), true, 0, -1) > -1) {
                        aIsThirdPartyContent = false;
                        statement->Finalize();
                        dbConn->Close();
                        return NS_OK;
                    }

                }
                aIsThirdPartyContent = true;
                statement->Finalize();
                dbConn->Close();


                return NS_OK;
            }

            static nsresult GetDomainType(nsCOMPtr<nsIURI> aURI, DomainType *type) {

                nsresult rv;
                nsAutoCString host;
                aURI->GetHost(host);

                nsAutoCString query;
                query.Assign("select count(1) as COUNT from websites where domain like '%");
                query.Append(host);
                query.Append("%';");

                nsCOMPtr<mozIStorageService> dbService =
                    do_GetService("@mozilla.org/storage/service;1", &rv);
                NS_ENSURE_SUCCESS(rv, rv);

                nsCOMPtr<nsIFile> dbFile;
                rv = getDBFile(getter_AddRefs(dbFile));
                NS_ENSURE_SUCCESS(rv, rv);

                nsCOMPtr<mozIStorageConnection> dbConn;
                rv = dbService->OpenDatabase(dbFile, getter_AddRefs(dbConn));

                if(NS_FAILED(rv))
                {
                    fprintf(stderr, "ARKTIK FOX FAILED 2 \n");
                }
                nsCOMPtr<mozIStorageStatement> statement;
                rv = dbConn->CreateStatement(query, getter_AddRefs(statement));
                NS_ENSURE_SUCCESS(rv, rv);

                bool hasMoreData;
                if (NS_SUCCEEDED(statement->ExecuteStep(&hasMoreData)) && hasMoreData) {
                    PRInt32 value;
                    rv = statement->GetInt32(0, &value);
                    NS_ENSURE_SUCCESS(rv, rv);
                    if(value > 0) {
                        *type = eFinancialDomain;
                    }
                    else {
                        *type = eOtherDomain;
                    }

                }
                statement->Finalize();
                dbConn->Close();

                return NS_OK;
            }

        private:
            static nsresult getDBFile(nsIFile **aOutDBFile) {
                NS_ENSURE_ARG_POINTER(aOutDBFile);
                *aOutDBFile = nullptr;

                char cwd[2500];
                char* result = getcwd(cwd, sizeof(cwd));
                if ( result == NULL)
                {
                    return NS_ERROR_FAILURE;
                }

                nsAutoString currentDir;
                currentDir.Append(NS_ConvertUTF8toUTF16(cwd));

                nsresult rv;
                nsCOMPtr<nsIFile> dbFile = do_CreateInstance("@mozilla.org/file/local;1", &rv);
                NS_ENSURE_SUCCESS(rv, rv);

                rv = dbFile->InitWithPath(currentDir);
                NS_ENSURE_SUCCESS(rv, rv);

                rv = dbFile->AppendRelativePath(NS_LITERAL_STRING("database/websites.sqlite"));
                NS_ENSURE_SUCCESS(rv, rv);

                NS_ADDREF(*aOutDBFile = dbFile);

                return NS_OK;

            }
    };
}
#endif
