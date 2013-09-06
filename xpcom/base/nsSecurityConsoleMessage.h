/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsSecurityConsoleMessage_h__
#define nsSecurityConsoleMessage_h__
#include "nsISecurityConsoleMessage.h"
#include "nsString.h"

class nsSecurityConsoleMessage : public nsISecurityConsoleMessage
{
  public:
    NS_DECL_ISUPPORTS
    NS_DECL_NSISECURITYCONSOLEMESSAGE

    nsSecurityConsoleMessage();

  private:
    virtual ~nsSecurityConsoleMessage();

  protected:
    nsString mLookupKey;
    nsString mCategory;
    nsString mSourceLine;
    nsString mSourceName;
    const PRUnichar** mParams;
    uint32_t mParamsLength;
    uint32_t mColumnNumber;
    uint32_t mLineNumber;
};

#define NS_SECURITY_CONSOLE_MESSAGE_CID \
  {0x43ebf210, 0x8a7b, 0x4ddb, {0xa8, 0x3d, 0xb8, 0x7c, 0x51, 0xa0, 0x58, 0xdb}}
#endif //nsSecurityConsoleMessage_h__
