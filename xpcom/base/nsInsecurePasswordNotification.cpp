/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsInsecurePasswordNotification.h"
#include "nsIURI.h"

NS_IMPL_ISUPPORTS1(nsInsecurePasswordNotification, nsIInsecurePasswordNotification)

nsInsecurePasswordNotification::nsInsecurePasswordNotification()
{
}

nsInsecurePasswordNotification::~nsInsecurePasswordNotification()
{
}

NS_IMETHODIMP nsInsecurePasswordNotification::GetOffenderType(nsAString& aOffenderType)
{
    aOffenderType = mOffenderType;
    return NS_OK;
}

NS_IMETHODIMP nsInsecurePasswordNotification::SetOffenderType(const nsAString& aOffenderType)
{
    mOffenderType = aOffenderType;
    return NS_OK;
}

NS_IMETHODIMP nsInsecurePasswordNotification::GetOffenderURL(nsAString& aOffenderURL)
{
    aOffenderURL = mOffenderURL;
    return NS_OK;
}

NS_IMETHODIMP nsInsecurePasswordNotification::SetOffenderURL(const nsAString& aOffenderURL)
{
    mOffenderURL = aOffenderURL;
    return NS_OK;
}

NS_IMETHODIMP nsInsecurePasswordNotification::Init(const nsAString& aOffenderType, const nsAString& aOffenderURL)
{
    mOffenderURL = aOffenderURL;
    mOffenderType = aOffenderType;
    return NS_OK;
}
