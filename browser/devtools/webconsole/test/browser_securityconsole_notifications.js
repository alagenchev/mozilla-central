 /* Any copyright is dedicated to the Public Domain.
  * http://creativecommons.org/publicdomain/zero/1.0/ */

// Test for Bug 897240
// Tests that security console notifications are sent successfuly.
 const TEST_URI = "https://example.com/browser/browser/devtools/webconsole/test/test-bug-846918-hsts-invalid-headers.html";

 function test()
 {
   observer.init();
   addTab(TEST_URI);
 }

 let observer = {
   QueryInterface: XPCOMUtils.generateQI([Ci.nsIObserver]),

   observe: function observe(aSubject, aTopic, aData)
   {
     aSubject = aSubject.QueryInterface(Ci.nsISecurityConsoleMessage);
     Services.obs.removeObserver(this, "security-console-message-received");
     ok("InvalidSTSHeaders" == aSubject.lookupKey, "We have the correct lookupKey");
     finishTest();
   },

   init: function init()
   {
     Services.obs.addObserver(this, "security-console-message-received", false);
   }
 };
