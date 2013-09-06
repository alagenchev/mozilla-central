/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsSecurityConsoleService_h__
#define nsSecurityConsoleService_h__
#include "nsISecurityConsoleService.h"

class nsSecurityConsoleService : public nsISecurityConsoleService
{
  public:
    NS_DECL_ISUPPORTS
    NS_DECL_NSISECURITYCONSOLESERVICE
    nsSecurityConsoleService();

  private:
    virtual ~nsSecurityConsoleService();
    nsresult SendObserverNotification(nsISecurityConsoleMessage* aMessage);
    nsresult GetLocalizedMessage(nsXPIDLString* aErrorText,
                                 nsISecurityConsoleMessage *aMessage);
};

#define NS_SECURITY_CONSOLESERVICE_CID \
{ 0xE55B4D1E, 0xFAE7, 0x11E2, { 0x95, 0x7E, 0x8A, 0x6E, 0x61, 0x88, 0x70, 0x9b }}
#endif //nsSecurityConsoleService_h__
