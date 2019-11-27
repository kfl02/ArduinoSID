#pragma once

#ifndef ARDUINOSID_SID_H
#define ARDUINOSID_SID_H

#include <array>
#include <functional>
#include <cstdint>
#include <cstddef>
#include <cassert>

#include "ringbuffer.h"


// representation of a SID chip with 3 voices and filter/vol and misc registers

class SID {

public:
    static const uint8_t NUM_VOICES = 3;
    static const uint8_t NUM_VOICE_REGS = 7;
    static const uint8_t NUM_FILTER_REGS = 4;
    static const uint8_t NUM_WO_REGS = NUM_VOICES * NUM_VOICE_REGS + NUM_FILTER_REGS;
    static const uint8_t NUM_RO_REGS = 4;
    static const uint8_t NUM_REGS = NUM_WO_REGS + NUM_RO_REGS;

protected:

    // registers for a single voice of a SID chip

    class SIDVoice {

    public:
        // register numbers
        static const uint8_t SIDRegFQLo  = 0;
        static const uint8_t SIDRegFQHi  = 1;
        static const uint8_t SIDRegPWLo  = 2;
        static const uint8_t SIDRegPWHi  = 3;
        static const uint8_t SIDRegWvCtl = 4;
        static const uint8_t SIDRegAD    = 5;
        static const uint8_t SIDRegSR    = 6;

        // waveform bits

        static const uint8_t SIDWavNse = 0x80;
        static const uint8_t SIDWavSqu = 0x40;
        static const uint8_t SIDWavSaw = 0x20;
        static const uint8_t SIDWavTri = 0x10;

        // control bits

        static const uint8_t SIDCtlTst = 0x08;
        static const uint8_t SIDCtlRMd = 0x04;
        static const uint8_t SIDCtlSyn = 0x02;
        static const uint8_t SIDCtlGat = 0x01;

    private:
        const uint8_t SIDNo;
        const uint8_t voiceNo;
        const uint8_t regOffset;

        uint8_t FQLo  = 0; // low byte of frequency
        uint8_t FQHi  = 0; // high byte of frequency
        uint8_t PWLo  = 0; // low byte of pulse width
        uint8_t PWHi  = 0; // high byte of pulse width (only lower nybble used)
        uint8_t WvCtl = 0; // wave and control register
        uint8_t AD    = 0; // attack and decay
        uint8_t SR    = 0; // sustain and release

        // callback function for register write actions
        std::function<void(const uint8_t, const uint8_t, const uint8_t)> registerWriteCallback = [](const uint8_t, const uint8_t, const uint8_t) {};

    public:
        SIDVoice(const uint8_t SIDNo, const uint8_t voiceNo)
            : SIDNo(SIDNo),
              voiceNo(voiceNo),
              regOffset((voiceNo & 0xf) * NUM_VOICES)) {
        }

        inline uint8_t const getVoiceNo() {
            return voiceNo;
        }

        const void setRegisterWriteCallback(const std::function<void(const uint8_t, const uint8_t, const uint8_t)> cb) {
            registerWriteCallback = cb;
        }

        inline uint8_t const getRegNo(const uint8_t reg) {
            assert(reg < NUM_VOICE_REGS);

            return regOffset + reg;
        }

        // frequency

        inline uint16_t const getFQ() {
            return (((uint16_t) FQHi) << 8) | FQLo;
        }

        inline void setFQ(const uint16_t FQ) {
            FQLo = (uint8_t) (FQ & 0xff);
            FQHi = (uint8_t) (FQ >> 8);

            registerWriteCallback(SIDNo, getRegNo(SIDRegFQLo), FQLo);
            registerWriteCallback(SIDNo, getRegNo(SIDRegFQHi), FQHi);
        }

        // pulse width

        inline uint16_t const getPW() {
            return (((uint16_t) PWHi) << 12) | (((uint16_t) PWLo) << 4);
        }

        inline void setPW(const uint16_t PW) {
            PWLo = (uint8_t) ((PW >> 4) & 0xff);
            PWHi = (uint8_t) (PW >> 12);

            registerWriteCallback(SIDNo, getRegNo(SIDRegPWLo), PWLo);
            registerWriteCallback(SIDNo, getRegNo(SIDRegPWHi), PWHi);
        }

        // waveforms

        inline uint8_t const getWave() {
            return WvCtl & 0xf0;
        }

        inline void setWave(const uint8_t wave) {
            WvCtl = (WvCtl & 0x0f) | (wave & 0xf0);

            registerWriteCallback(SIDNo, getRegNo(SIDRegWvCtl), WvCtl);
        }

        inline bool const getNoise() {
            return WvCtl & SIDWavNse;
        }

        inline void setNoise(const bool onOff) {
            WvCtl = (WvCtl & ~SIDWavNse) | (onOff ? SIDWavNse : 0);

            registerWriteCallback(SIDNo, getRegNo(SIDRegWvCtl), WvCtl);
        }

        inline bool const getSquare() {
            return WvCtl & SIDWavSqu;
        }

        inline void setSquare(const bool onOff) {
            WvCtl = (WvCtl & ~SIDWavSqu) | (onOff ? SIDWavSqu : 0);

            registerWriteCallback(SIDNo, getRegNo(SIDRegWvCtl), WvCtl);
        }

        inline bool const getSawtooth() {
            return WvCtl & SIDWavSaw;
        }

        inline void setSawtooth(const bool onOff) {
            WvCtl = (WvCtl & ~SIDWavSaw) | (onOff ? SIDWavSaw : 0);

            registerWriteCallback(SIDNo, getRegNo(SIDRegWvCtl), WvCtl);
        }

        inline bool const getTriangle() {
            return WvCtl & SIDWavTri;
        }

        inline void setTriangle(const bool onOff) {
            WvCtl = (WvCtl & ~SIDWavTri) | (onOff ? SIDWavTri : 0);

            registerWriteCallback(SIDNo, getRegNo(SIDRegWvCtl), WvCtl);
        }

        // control bits

        inline uint8_t const getControl() {
            return WvCtl & 0x0f;
        }

        inline void setControl(const uint8_t control) {
            WvCtl = (WvCtl & 0xf0) | (control & 0x0f);

            registerWriteCallback(SIDNo, getRegNo(SIDRegWvCtl), WvCtl);
        }

        inline bool const getTest() {
            return WvCtl & SIDCtlTst;
        }

        inline void setTest(const bool test) {
            WvCtl = (WvCtl & ~SIDCtlTst) | (test ? SIDCtlTst : 0);

            registerWriteCallback(SIDNo, getRegNo(SIDRegWvCtl), WvCtl);
        }

        inline bool const getRingMod() {
            return WvCtl & SIDCtlRMd;
        }

        inline void setRingMod(const bool ringMod) {
            WvCtl = (WvCtl & ~SIDCtlRMd) | (ringMod ? SIDCtlRMd : 0);

            registerWriteCallback(SIDNo, getRegNo(SIDRegWvCtl), WvCtl);
        }

        inline bool const getSync() {
            return WvCtl & SIDCtlSyn;
        }

        inline void setSync(const bool sync) {
            WvCtl = (WvCtl & ~SIDCtlSyn) | (sync ? SIDCtlSyn : 0);

            registerWriteCallback(SIDNo, getRegNo(SIDRegWvCtl), WvCtl);
        }

        inline bool const getGate() {
            return WvCtl & SIDCtlGat;
        }

        inline void setGate(const bool gate) {
            WvCtl = (WvCtl & ~SIDCtlGat) | (gate ? SIDCtlGat : 0);

            registerWriteCallback(SIDNo, getRegNo(SIDRegWvCtl), WvCtl);
        }

        // envelope

        inline uint8_t const getAD() {
            return AD;
        }

        inline void setAD(const uint8_t AD) {
            this->AD = AD;

            registerWriteCallback(SIDNo, getRegNo(SIDRegAD), this->AD);
        }

        inline uint8_t const getSR() {
            return SR;
        }

        inline void setSR(const uint8_t SR) {
            this->SR = SR;

            registerWriteCallback(SIDNo, getRegNo(SIDRegSR), this->SR);
        }

        inline uint16_t const getADSR() {
            return ((uint16_t) AD << 8) | (uint16_t) SR;
        }

        inline void setADSR(const uint16_t ADSR) {
            AD = (uint8_t) (ADSR >> 8);
            SR = (uint8_t) (ADSR & 0xff);

            registerWriteCallback(SIDNo, getRegNo(SIDRegAD), AD);
            registerWriteCallback(SIDNo, getRegNo(SIDRegSR), SR);
        }

        inline uint8_t const getAttack() {
            return (AD & 0xf0) >> 4;
        }

        inline void setAttack(const uint8_t attack) {
            assert(attack <= 0x0f);

            AD = (AD & 0x0f) | (attack << 4);

            registerWriteCallback(SIDNo, getRegNo(SIDRegAD), AD);
        }

        inline uint8_t const getDecay() {
            return (AD & 0x0f);
        }

        inline void setDecay(const uint8_t decay) {
            assert(decay <= 0x0f);

            AD = (AD & 0xf0) | decay;

            registerWriteCallback(SIDNo, getRegNo(SIDRegAD), AD);
        }

        inline uint8_t const getSustain() {
            return (AD & 0xf0) >> 4;
        }

        inline void setSustain(const uint8_t sustain) {
            assert(sustain <= 0x0f);

            SR = (SR & 0x0f) | (sustain << 4);

            registerWriteCallback(SIDNo, getRegNo(SIDRegSR), SR);
        }

        inline uint8_t const getRelease() {
            return (SR & 0x0f);
        }

        inline void setRelease(const uint8_t release) {
            assert(release <= 0x0f);

            SR = (SR & 0xf0) | release;

            registerWriteCallback(SIDNo, getRegNo(SIDRegSR), SR);
        }
    };

    // filter and volume registers of a SID chip

    class SIDFilter {
    public:
        // register numbers
        static const uint8_t SIDRegFCLo    = NUM_VOICES * NUM_VOICE_REGS + 0;
        static const uint8_t SIDRegFCHi    = NUM_VOICES * NUM_VOICE_REGS + 1;
        static const uint8_t SIDRegResFilt = NUM_VOICES * NUM_VOICE_REGS + 2;
        static const uint8_t SIDRegModVol  = NUM_VOICES * NUM_VOICE_REGS + 3;

        static const uint8_t SIDFilt1  = 0x01;
        static const uint8_t SIDFilt2  = 0x02;
        static const uint8_t SIDFilt3  = 0x04;
        static const uint8_t SIDFiltEx = 0x08;

        static const uint8_t SIDFModLP = 0x10;
        static const uint8_t SIDFModBP = 0x20;
        static const uint8_t SIDFModHP = 0x40;

    private:
        const uint8_t SIDNo;

        uint8_t FCLo    = 0; // low byte filter cut-off frequency, only bits 1 to 3 are used
        uint8_t FCHi    = 0; // high byte filter cut-off frequency
        uint8_t ResFilt = 0; // filter resonance and control register
        uint8_t ModVol  = 0; // filter mode and chip volume

        // callback function for register write actions
        std::function<void(const uint8_t, const uint8_t, const uint8_t)> registerWriteCallback = [](const uint8_t, const uint8_t, const uint8_t) {};

    public:
        SIDFilter(const uint8_t SIDNo) : SIDNo(SIDNo) {
        }

        void const setRegisterWriteCallback(std::function<void(const uint8_t, const uint8_t, const uint8_t)> cb) {
            registerWriteCallback = cb;
        }

        // filter frequency

        inline uint16_t const getFilterFQ() {
            return ((uint16_t) FCLo << 5) | ((uint16_t) FCHi << 8);
        }

        inline void setFilterFQ(const uint16_t FQ) {
            FCHi = FQ & 0xff00;
            FCLo = (FQ & 0x00ff) >> 5;

            registerWriteCallback(SIDNo, SIDRegFCHi, FCHi);
            registerWriteCallback(SIDNo, SIDRegFCLo, FCLo);
        }

        // filter resonance

        inline uint16_t const getFilterRes() {
            return ResFilt & 0xf0;
        }

        inline void setFilterRes(const uint8_t res) {
            ResFilt = (ResFilt & 0x0f) | (res & 0xf0);

            registerWriteCallback(SIDNo, SIDRegResFilt, ResFilt);
        }

        // filter on/off flags

        inline bool const getFilter1() {
            return ResFilt & SIDFilt1;
        }

        inline void setFilter1(const bool onOff) {
            ResFilt = (ResFilt & ~SIDFilt1) | (onOff ? SIDFilt1 : 0);

            registerWriteCallback(SIDNo, SIDRegResFilt, ResFilt);
        }

        inline bool const getFilter2() {
            return ResFilt & SIDFilt2;
        }

        inline void setFilter2(const bool onOff) {
            ResFilt = (ResFilt & ~SIDFilt2) | (onOff ? SIDFilt2 : 0);

            registerWriteCallback(SIDNo, SIDRegResFilt, ResFilt);
        }

        inline bool const getFilter3() {
            return ResFilt & SIDFilt3;
        }

        inline void setFilter3(const bool onOff) {
            ResFilt = (ResFilt & ~SIDFilt3) | (onOff ? SIDFilt3 : 0);

            registerWriteCallback(SIDNo, SIDRegResFilt, ResFilt);
        }

        inline bool const getFilterEx() {
            return ResFilt & SIDFiltEx;
        }

        inline void setFilterEx(const bool onOff) {
            ResFilt = (ResFilt & ~SIDFiltEx) | (onOff ? SIDFiltEx : 0);

            registerWriteCallback(SIDNo, SIDRegResFilt, ResFilt);
        }

        inline bool const getFilter(const uint8_t voice) {
            assert(voice < 3);

            return ResFilt & (1 << voice);
        }

        inline void setFilter(const uint8_t voice, const bool onOff) {
            assert(voice < 3);

            uint8_t bit = (1 << voice);
            ResFilt = (ResFilt & ~bit) | (onOff ? bit : 0);

            registerWriteCallback(SIDNo, SIDRegResFilt, ResFilt);
        }

        // filter modes

        inline uint8_t const getFilterMode() {
            return ModVol & 0x70;
        }

        inline void setFilterMode(const uint8_t mode) {
            ModVol = (ModVol & 0x0f) | (mode & 0x70);

            registerWriteCallback(SIDNo, SIDRegModVol, ModVol);
        }

        inline void setFilterMode(const uint8_t mode, const bool onOff) {
            assert(mode == SIDFModHP || mode == SIDFModBP || mode == SIDFModLP);

            ModVol = (ModVol & ~mode) | (onOff ? mode : 0);

            registerWriteCallback(SIDNo, SIDRegModVol, ModVol);
        }

        // volume

        inline uint8_t const getVolume() {
            return ModVol & 0x0f;
        }

        inline void setVolume(const uint8_t volume) {
            assert(volume <= 0x0f);

            ModVol = (ModVol & 0xf0) | volume;

            registerWriteCallback(SIDNo, SIDRegModVol, ModVol);
        }
    };

    // read only registers of a SID chip, not implemented

    class SIDMisc {

    private:
        const uint8_t SIDNo;

        uint8_t PotX = 0;
        uint8_t PotY = 0;
        uint8_t Osc3 = 0;
        uint8_t Env3 = 0;

    public:
        SIDMisc(const uint8_t SIDNo) : SIDNo(SIDNo) {
        }

        inline uint8_t const getPotX() {
            return PotX;
        }

        inline uint8_t const getPotY() {
            return PotY;
        }

        inline uint8_t const getOsc3() {
            return Osc3;
        }

        inline uint8_t const getEnv3() {
            return Env3;
        }
    };

private:
    const uint8_t SIDNo;

    std::array<SIDVoice, NUM_VOICES> voices = { SIDVoice(SIDNo, 0), SIDVoice(SIDNo, 1), SIDVoice(SIDNo, 2) };
    SIDFilter filter = SIDFilter(SIDNo);
    SIDMisc misc = SIDMisc(SIDNo);

public:
    SID(const uint8_t SIDNo) : SIDNo(SIDNo) {
    }

    inline uint8_t const getSIDNo() {
        return SIDNo;
    }

    inline SIDVoice& getVoice(const uint8_t voiceNo) {
        assert(voiceNo < NUM_VOICES);

        return voices[voiceNo];
    }

    inline SIDFilter& getFilter() {
        return filter;
    }

    inline SIDMisc& getMisc() {
        return misc;
    }
};

// an array of SID chips

class SIDArray {
public:
    static const uint8_t MAX_NUM_SIDS = 6;

private:
    // ring buffer for saving register write actions to be processed by Arduino timer
    RingBuffer<std::tuple<uint8_t, uint8_t, uint8_t>, MAX_NUM_SIDS * SID::NUM_WO_REGS> buffer;

    // array of SID chips
    std::array<SID, MAX_NUM_SIDS> SIDs = { 0, 1, 2, 3, 4, 5 };

    // callback function for register writes
    static const void
    ringBufferCallback(RingBuffer<std::tuple<uint8_t, uint8_t, uint8_t>, MAX_NUM_SIDS * SID::NUM_WO_REGS> &buffer,
            const int busyWait, const int uint8_t sid, const uint8_t reg, const uint8_t val) {
        // if busyWait flag is true, loop until buffer is not full
        if(busyWait) {
            while(buffer.full()) {
            }
        }

        buffer.put(std::tuple<uint_t, uint8_t, uint8_t>(reg, val));
    }

public:
    SIDArray(bool busyWait = false) {
        // set up fixed parameters for register write callback
        auto cb = std::bind(ringBufferCallback, buffer, busyWait, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        // initialize register write callback for all SIDs and their voices
        for(uint8_t i = 0; i < MAX_NUM_SIDS; i++) {
            SID& sid = getSID(i);

            sid.getFilter().setRegisterWriteCallback(cb);

            for(uint8_t j = 0; j < SID::NUM_VOICES; j++) {
                sid.getVoice(j).setRegisterWriteCallback(cb);
            }
        }
    }

    SID& getSID(uint8_t SIDNo) {
        assert(SIDNo < MAX_NUM_SIDS);

        return SIDs[SIDNo];
    }

    auto &getRingBuffer() {
        return buffer;
    }
};

#endif // ARDUINOSID_SID_H
