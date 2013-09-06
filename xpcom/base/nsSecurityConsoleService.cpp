/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsISecurityConsoleMessage.h"
#include "nsStringGlue.h"
#include "nsIScriptError.h"
#include "nsContentUtils.h"
#include "nsIConsoleService.h"
#include "nsIDocument.h"
#include "nsSecurityConsoleService.h"
#include "nsIObserverService.h"
#include "mozilla/Services.h"
#include "nsComponentManagerUtils.h"
#include "nsServiceManagerUtils.h"

using namespace mozilla;

NS_IMPL_ISUPPORTS1(nsSecurityConsoleService, nsISecurityConsoleService)

nsSecurityConsoleService::nsSecurityConsoleService()
{
}

nsSecurityConsoleService::~nsSecurityConsoleService()
{
}

// Sends the provided nsISecurityConsoleMessage with an observer topic
// to the observer service
// Input:
//      aMessage - the nsISecurityConsoleMessage that is to be passed
//      to the observers with the "security-console-message-received"
//      observer topic.
nsresult
nsSecurityConsoleService::SendObserverNotification(nsISecurityConsoleMessage* aMessage)
{
  nsCOMPtr<nsIObserverService> observerService =
    mozilla::services::GetObserverService();
  if (!observerService) {
    return NS_ERROR_FAILURE;
  }

  observerService->NotifyObservers(aMessage,
      "security-console-message-received", NULL);
  return NS_OK;
}

// Creates a localized error message based on the passed
// nsISecurityConsoleMessage.
// Input:
//      aMessage - The nsISecurityConsoleMessage to be used for localizing the
//      error message.
//      aErrorText - Stores the localized error text after the operations are
//      complete.
nsresult
nsSecurityConsoleService::GetLocalizedMessage(nsXPIDLString* aErrorText,
                                              nsISecurityConsoleMessage *aMessage)
{

  uint32_t paramsLength;
  nsAutoString messageLookupKey;
  const PRUnichar** params;

  aMessage->GetLookupKey(messageLookupKey);
  aMessage->GetParamsLength(&paramsLength);
  aMessage->GetParams(&params);

  NS_ASSERTION((params && paramsLength) || (!params && !paramsLength),
               "Supply either both parameters and their number or no parameters and 0.");

  if (paramsLength > 0 && params) {
    return nsContentUtils::FormatLocalizedString(nsContentUtils::eSECURITY_PROPERTIES,
                                                 NS_ConvertUTF16toUTF8(messageLookupKey).get(),
                                                 params, paramsLength, *aErrorText);
  }
  else {
    return nsContentUtils::GetLocalizedString(nsContentUtils::eSECURITY_PROPERTIES,
                                              NS_ConvertUTF16toUTF8(messageLookupKey).get(),
                                              *aErrorText);
  }
}

// Logs the passed nsISecurityConsoleMessage to the console associated with
// the document that has the specified innerWindowID and sends a notification
// to the observer service using "security-console-message-received" topic.
// Input:
//      aMessage - The message to be logged to the console.
//      aInnerWindowID - The inner window id of the document where we will log
//      the message.
NS_IMETHODIMP
nsSecurityConsoleService::LogMessage(nsISecurityConsoleMessage *aMessage,
                                     uint64_t aInnerWindowID)
{
  nsresult rv;
  nsXPIDLString errorText;
  nsAutoString messageLookupKey;
  nsAutoString category;
  nsAutoString sourceName;
  nsAutoString sourceLine;
  uint32_t lineNumber;
  uint32_t columnNumber;

  aMessage->GetLookupKey(messageLookupKey);
  aMessage->GetCategory(category);
  aMessage->GetSourceName(sourceName);
  aMessage->GetSourceLine(sourceLine);
  aMessage->GetLineNumber(&lineNumber);
  aMessage->GetColumnNumber(&columnNumber);

  nsCOMPtr<nsIScriptError> errorObject =
    do_CreateInstance(NS_SCRIPTERROR_CONTRACTID, &rv);
  if (!errorObject) {
    return NS_ERROR_FAILURE;
  }

  rv = GetLocalizedMessage(&errorText, aMessage);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = errorObject->InitWithWindowID(errorText, sourceName, sourceLine,
                                     lineNumber, columnNumber,
                                     nsIScriptError::warningFlag,
                                     NS_ConvertUTF16toUTF8(category),
                                     aInnerWindowID);
  NS_ENSURE_SUCCESS(rv, rv);

  nsCOMPtr<nsIConsoleService> consoleService =
    do_GetService(NS_CONSOLESERVICE_CONTRACTID);
  if (consoleService) {
    consoleService->LogMessage(errorObject);
    rv = SendObserverNotification(aMessage);
    NS_ENSURE_SUCCESS(rv, rv);
  }

  return NS_OK;
}
