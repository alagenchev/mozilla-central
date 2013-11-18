/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsInsecurePasswordNotification_h__
#define nsInsecurePasswordNotification_h__
#include "nsIInsecurePasswordNotification.h"
#include "nsString.h"

class nsInsecurePasswordNotification MOZ_FINAL : public nsIInsecurePasswordNotification
{
  public:
    NS_DECL_ISUPPORTS
    NS_DECL_NSIINSECUREPASSWORDNOTIFICATION

    nsInsecurePasswordNotification();
  private:
    ~nsInsecurePasswordNotification();

  protected:
    nsString mOffenderType;
    nsString mOffenderURL;
};

#define NS_INSECURE_PASSWORD_NOTIFICATION_CID \
  {0xdfc7fe8e, 0x7a58, 0x424b, {0xb3, 0x2e, 0x89, 0xae, 0x8c, 0x78, 0x0d, 0x5d}}
#endif //nsInsecurePasswordNotification_h__
