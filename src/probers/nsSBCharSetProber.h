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

#ifndef NSSBCHARSETPROBER_H
#define NSSBCHARSETPROBER_H

#include "nsCharSetProber.h"

#define SAMPLE_SIZE 64
#define SB_ENOUGH_REL_THRESHOLD  1024
#define POSITIVE_SHORTCUT_THRESHOLD  (float)0.95
#define NEGATIVE_SHORTCUT_THRESHOLD  (float)0.05
#define SYMBOL_CAT_ORDER  250
#define NUMBER_OF_SEQ_CAT 4
#define POSITIVE_CAT   (NUMBER_OF_SEQ_CAT-1)
#define NEGATIVE_CAT   0

namespace kencodingprober
{
typedef struct {
    const unsigned char *charToOrderMap;    // [256] table use to find a char's order
    const char *precedenceMatrix;           // [SAMPLE_SIZE][SAMPLE_SIZE]; table to find a 2-char sequence's frequency
    float  mTypicalPositiveRatio;     // = freqSeqs / totalSeqs
    bool keepEnglishLetter;         // says if this script contains English characters (not implemented)
    const char *charsetName;
} SequenceModel;

class KCODECS_NO_EXPORT nsSingleByteCharSetProber : public nsCharSetProber
{
public:
    nsSingleByteCharSetProber(const SequenceModel *model)
        : mModel(model), mReversed(false), mNameProber(nullptr)
    {
        Reset();
    }
    nsSingleByteCharSetProber(const SequenceModel *model, bool reversed, nsCharSetProber *nameProber)
        : mModel(model), mReversed(reversed), mNameProber(nameProber)
    {
        Reset();
    }

    const char *GetCharSetName() override;
    nsProbingState HandleData(const char *aBuf, unsigned int aLen) override;
    nsProbingState GetState(void) override
    {
        return mState;
    }
    void      Reset(void) override;
    float     GetConfidence(void) override;
    void      SetOpion() override {}

    // This feature is not implemented yet. any current language model
    // contain this parameter as false. No one is looking at this
    // parameter or calling this method.
    // Moreover, the nsSBCSGroupProber which calls the HandleData of this
    // prober has a hard-coded call to FilterWithoutEnglishLetters which gets rid
    // of the English letters.
    bool KeepEnglishLetters()
    {
        return mModel->keepEnglishLetter;
    } // (not implemented)

#ifdef DEBUG_PROBE
    void  DumpStatus() override;
#endif

protected:
    nsProbingState mState;
    const SequenceModel *mModel;
    const bool mReversed; // true if we need to reverse every pair in the model lookup

    //char order of last character
    unsigned char mLastOrder;

    unsigned int mTotalSeqs;
    unsigned int mSeqCounters[NUMBER_OF_SEQ_CAT];

    unsigned int mTotalChar;
    //characters that fall in our sampling range
    unsigned int mFreqChar;

    // Optional auxiliary prober for name decision. created and destroyed by the GroupProber
    nsCharSetProber *mNameProber;

};

extern const SequenceModel Koi8rModel;
extern const SequenceModel Win1251Model;
extern const SequenceModel Latin5Model;
extern const SequenceModel MacCyrillicModel;
extern const SequenceModel Ibm866Model;
extern const SequenceModel Ibm855Model;
extern const SequenceModel Latin7Model;
extern const SequenceModel Win1253Model;
extern const SequenceModel Latin5BulgarianModel;
extern const SequenceModel Win1251BulgarianModel;
extern const SequenceModel Latin2HungarianModel;
extern const SequenceModel Win1250HungarianModel;
extern const SequenceModel Win1255Model;
}
#endif /* NSSBCHARSETPROBER_H */

