/* -*- Mode: Java; c-basic-offset: 4; tab-width: 20; indent-tabs-mode: nil; -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.mozilla.gecko;

import android.content.Context;
import android.provider.Settings.Secure;
import android.view.inputmethod.InputMethodInfo;
import android.view.inputmethod.InputMethodManager;

import java.util.Collection;

final class InputMethods {

    public static final String METHOD_SWYPE = "com.swype.android.inputmethod/.SwypeInputMethod";
    public static final String METHOD_SWYPE_BETA = "com.nuance.swype.input/.IME";

    /* These input method names are currently unused, but kept here for future reference:
    public static final String METHOD_EYES_FREE_KEYBOARD = "com.googlecode.eyesfree.inputmethod.latin/.LatinIME";
    public static final String METHOD_GO_KEYBOARD = "com.jb.gokeyboard/.GoKeyboard";
    public static final String METHOD_GOOGLE_JAPANESE_INPUT = "com.google.android.inputmethod.japanese/.MozcService";
    public static final String METHOD_GOOGLE_PINYIN = "com.google.android.inputmethod.pinyin/.PinyinIME";
    public static final String METHOD_GOOGLE_TALKBACK = "com.google.android.marvin.talkback/com.googlecode.eyesfree.inputmethod.latin.LatinIME";
    public static final String METHOD_HACKERS_KEYBOARD = "org.pocketworkstation.pckeyboard/.LatinIME";
    public static final String METHOD_OPENWNN_PLUS = "com.owplus.ime.openwnnplus/.OpenWnnJAJP";
    public static final String METHOD_SAMSUNG_GALAXY_NOTE = "com.samsung.sec.android.inputmethod.axt9/.AxT9IME";
    public static final String METHOD_SIMEJI = "com.adamrocker.android.input.simeji/.OpenWnnSimeji";
    public static final String METHOD_SLIDE_IT_KEYBOARD = "com.dasur.slideit.vt.lite/com.dasur.slideit.SlideITIME";
    public static final String METHOD_SWIFTKEY_TRIAL = "com.touchtype.swiftkey.phone.trial/com.touchtype.KeyboardService";
    public static final String METHOD_TOUCHPAL_KEYBOARD = "com.cootek.smartinputv5/.TouchPalIME";
    */

    private InputMethods() {}

    public static String getCurrentInputMethod(Context context) {
        return Secure.getString(context.getContentResolver(), Secure.DEFAULT_INPUT_METHOD);
    }

    public static InputMethodInfo getInputMethodInfo(Context context, String inputMethod) {
        InputMethodManager imm = getInputMethodManager(context);
        Collection<InputMethodInfo> infos = imm.getEnabledInputMethodList();
        for (InputMethodInfo info : infos) {
            if (info.getId().equals(inputMethod)) {
                return info;
            }
        }
        return null;
    }

    public static InputMethodManager getInputMethodManager(Context context) {
        return (InputMethodManager) context.getSystemService(Context.INPUT_METHOD_SERVICE);
    }
}
