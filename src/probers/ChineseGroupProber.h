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

#ifndef CHINESEGROUPPROBER_H
#define CHINESEGROUPPROBER_H

#include "nsCharSetProber.h"

#define CN_NUM_OF_PROBERS    3
namespace kencodingprober
{
class KCODECS_NO_EXPORT ChineseGroupProber: public nsCharSetProber
{
public:
    ChineseGroupProber();
    virtual ~ChineseGroupProber();
    nsProbingState HandleData(const char *aBuf, unsigned int aLen) override;
    const char *GetCharSetName() override;
    nsProbingState GetState(void) override
    {
        return mState;
    }
    void      Reset(void) override;
    float     GetConfidence(void) override;
    void      SetOpion() override {}

#ifdef DEBUG_PROBE
    void  DumpStatus() override;
#endif

protected:
    nsProbingState mState;
    nsCharSetProber *mProbers[CN_NUM_OF_PROBERS];
    bool          mIsActive[CN_NUM_OF_PROBERS];
    int mBestGuess;
    unsigned int mActiveNum;
};
}
#endif /* CHINESEGROUPPROBER_H */

