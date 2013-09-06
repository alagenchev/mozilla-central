/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsSecurityConsoleMessage.h"

NS_IMPL_ISUPPORTS1(nsSecurityConsoleMessage, nsISecurityConsoleMessage)

nsSecurityConsoleMessage::nsSecurityConsoleMessage()
{
  mParams = NULL;
}

nsSecurityConsoleMessage::~nsSecurityConsoleMessage()
{
}

NS_IMETHODIMP
nsSecurityConsoleMessage::GetLookupKey(nsAString& aLookupKey)
{
  aLookupKey = mLookupKey;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::SetLookupKey(const nsAString& aLookupKey)
{
  mLookupKey = aLookupKey;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::GetParamsLength(uint32_t *aParamsLength)
{
  *aParamsLength = mParamsLength;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::SetParamsLength(uint32_t aParamsLength)
{
  mParamsLength = aParamsLength;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::GetParams(const PRUnichar*** aParams)
{
  *aParams = mParams;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::SetParams(const PRUnichar** aParams)
{
  mParams = aParams;
  return NS_OK;
}


NS_IMETHODIMP
nsSecurityConsoleMessage::GetCategory(nsAString& aCategory)
{
  aCategory = mCategory;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::SetCategory(const nsAString& aCategory)
{
  mCategory = aCategory;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::GetSourceLine(nsAString& aSourceLine)
{
  aSourceLine = mSourceLine;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::SetSourceLine(const nsAString& aSourceLine)
{
  mSourceLine = aSourceLine;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::GetSourceName(nsAString& aSourceName)
{
  aSourceName = mSourceName;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::SetSourceName(const nsAString& aSourceName)
{
  mSourceName = aSourceName;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::GetLineNumber(uint32_t *aLineNumber)
{
  *aLineNumber = mLineNumber;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::SetLineNumber(uint32_t aLineNumber)
{
  mLineNumber = aLineNumber;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::GetColumnNumber(uint32_t *aColumnNumber)
{
  *aColumnNumber = mColumnNumber;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::SetColumnNumber(uint32_t aColumnNumber)
{
  mColumnNumber = aColumnNumber;
  return NS_OK;
}

NS_IMETHODIMP
nsSecurityConsoleMessage::Init(const nsAString & aLookupKey,
                               const nsAString& aCategory,
                               const nsAString& aSourceLine,
                               const nsAString& aSourceName,
                               uint32_t aLineNumber,
                               uint32_t aColumnNumber,
                               const PRUnichar** aParams,
                               uint32_t aParamsLength)
{
  mLookupKey = aLookupKey;
  mCategory = aCategory;
  mSourceLine = aSourceLine;
  mSourceName = aSourceName;
  mLineNumber = aLineNumber;
  mColumnNumber = aColumnNumber;
  mParams = aParams;
  mParamsLength = aParamsLength;
  return NS_OK;
}
