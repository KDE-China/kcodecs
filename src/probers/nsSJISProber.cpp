/*  -*- C++ -*-
*  Copyright (C) 1998 <developer@mozilla.org>
*
*
*  Permission is hereby granted, free of charge, to any person obtaining
*  a copy of this software and associated documentation files (the
*  "Software"), to deal in the Software without restriction, including
*  without limitation the rights to use, copy, modify, merge, publish,
*  distribute, sublicense, and/or sell copies of the Software, and to
*  permit persons to whom the Software is furnished to do so, subject to
*  the following conditions:
*
*  The above copyright notice and this permission notice shall be included
*  in all copies or substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
*  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
*  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
*  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// for S-JIS encoding, obeserve characteristic:
// 1, kana character (or hankaku?) often have hight frequency of appereance
// 2, kana character often exist in group
// 3, certain combination of kana is never used in japanese language

#include "nsSJISProber.h"

namespace kencodingprober
{
void  nsSJISProber::Reset(void)
{
    mCodingSM->Reset();
    mState = eDetecting;
    mContextAnalyser.Reset();
    mDistributionAnalyser.Reset();
}

nsProbingState nsSJISProber::HandleData(const char *aBuf, unsigned int aLen)
{
    nsSMState codingState;

    for (unsigned int i = 0; i < aLen; i++) {
        codingState = mCodingSM->NextState(aBuf[i]);
        if (codingState == eError) {
            mState = eNotMe;
            break;
        }
        if (codingState == eItsMe) {
            mState = eFoundIt;
            break;
        }
        if (codingState == eStart) {
            unsigned int charLen = mCodingSM->GetCurrentCharLen();
            if (i == 0) {
                mLastChar[1] = aBuf[0];
                mContextAnalyser.HandleOneChar(mLastChar + 2 - charLen, charLen);
                mDistributionAnalyser.HandleOneChar(mLastChar, charLen);
            } else {
                mContextAnalyser.HandleOneChar(aBuf + i + 1 - charLen, charLen);
                mDistributionAnalyser.HandleOneChar(aBuf + i - 1, charLen);
            }
        }
    }

    mLastChar[0] = aBuf[aLen - 1];

    if (mState == eDetecting)
        if (mContextAnalyser.GotEnoughData() && GetConfidence() > SHORTCUT_THRESHOLD) {
            mState = eFoundIt;
        }

    return mState;
}

float nsSJISProber::GetConfidence(void)
{
    float contxtCf = mContextAnalyser.GetConfidence();
    float distribCf = mDistributionAnalyser.GetConfidence();

    return (contxtCf > distribCf ? contxtCf : distribCf);
}
}

