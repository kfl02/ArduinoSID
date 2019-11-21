#pragma once

#ifndef ARDUINOSID_SID_H
#define ARDUINOSID_SID_H

#include <array>
#include <functional>
#include <cstdint>
#include <cstddef>
#include <cassert>

#include "ringbuffer.h"

static const uint8_t SID_ARRAY_MAX_NUM_SIDS = 6;

static const uint8_t SID_NUM_VOICES = 3;
static const uint8_t SID_NUM_VOICE_REGS = 6;
static const uint8_t SID_NUM_FILTER_REGS = 4;
static const uint8_t SID_NUM_WO_REGS = SID_NUM_VOICES * SID_NUM_VOICE_REGS + SID_NUM_FILTER_REGS;
static const uint8_t SID_NUM_RO_REGS = 4;
static const uint8_t SID_NUM_REGS = SID_NUM_WO_REGS + SID_NUM_RO_REGS;

// representation of a SID chip with 3 voices and filter/vol and misc registers

class SID {
    class SIDVoice {
    // registers for a single voice of a SID chip

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
        const uint8_t voiceNo; // high nybble: SID number, low nybble: voice number, set from SID
        const uint8_t regOffset;

        uint8_t FQLo  = 0; // low byte of frequency
        uint8_t FQHi  = 0; // high byte of frequency
        uint8_t PWLo  = 0; // low byte of pulse width
        uint8_t PWHi  = 0; // high byte of pulse width (only lower nybble used)
        uint8_t WvCtl = 0; // wave and control register
        uint8_t AD    = 0; // attack and decay
        uint8_t SR    = 0; // sustain and release

        // callback function for register write actions
        std::function<void(const uint8_t, const uint8_t)> registerWriteCallback = [](const uint8_t, const uint8_t) {};

    public:
        SIDVoice(const uint8_t SIDNo, const uint8_t voiceNo)
            : voiceNo((SIDNo << 4) | (voiceNo & 0xf)),
              regOffset((SIDNo << 4) | ((voiceNo & 0xf) * SID_NUM_VOICES)) {
        }

        uint8_t const getVoiceNo() {
            return voiceNo;
        }

        const void setRegisterWriteCallback(const std::function<void(const uint8_t, const uint8_t)> cb) {
            registerWriteCallback = cb;
        }

        uint8_t const getRegNo(const uint8_t reg) {
            assert(reg < SID_NUM_VOICE_REGS);

            return regOffset + reg;
        }

        uint16_t const getFQ() {
            return (((uint16_t) FQHi) << 8) | FQLo;
        }

        void setFQ(const uint16_t FQ) {
            FQLo = (uint8_t) (FQ & 0xff);
            FQHi = (uint8_t) (FQ >> 8);

            registerWriteCallback(getRegNo(SIDRegFQLo), FQLo);
            registerWriteCallback(getRegNo(SIDRegFQHi), FQHi);
        }

        uint16_t const getPW() {
            return (((uint16_t) PWHi) << 12) | (((uint16_t) PWLo) << 4);
        }

        void setPW(const uint16_t PW) {
            PWLo = (uint8_t) ((PW >> 4) & 0xff);
            PWHi = (uint8_t) (PW >> 12);

            registerWriteCallback(getRegNo(SIDRegPWLo), PWLo);
            registerWriteCallback(getRegNo(SIDRegPWHi), PWHi);
        }

        uint8_t const getWave() {
            return WvCtl & 0xf0;
        }

        void setWave(const uint8_t wave) {
            WvCtl = (WvCtl & 0x0f) | (wave & 0xf0);

            registerWriteCallback(getRegNo(SIDRegWvCtl), WvCtl);
        }

        bool const getNoise() {
            return WvCtl & SIDWavNse;
        }

        void setNoise(const bool onOff) {
            WvCtl = (WvCtl & ~SIDWavNse) | (onOff ? SIDWavNse : 0);

            registerWriteCallback(getRegNo(SIDRegWvCtl), WvCtl);
        }

        bool const getSquare() {
            return WvCtl & SIDWavSqu;
        }

        void setSquare(const bool onOff) {
            WvCtl = (WvCtl & ~SIDWavSqu) | (onOff ? SIDWavSqu : 0);

            registerWriteCallback(getRegNo(SIDRegWvCtl), WvCtl);
        }

        bool const getSawtooth() {
            return WvCtl & SIDWavSaw;
        }

        void setSawtooth(const bool onOff) {
            WvCtl = (WvCtl & ~SIDWavSaw) | (onOff ? SIDWavSaw : 0);

            registerWriteCallback(getRegNo(SIDRegWvCtl), WvCtl);
        }

        bool const getTriangle() {
            return WvCtl & SIDWavTri;
        }

        void setTriangle(const bool onOff) {
            WvCtl = (WvCtl & ~SIDWavTri) | (onOff ? SIDWavTri : 0);

            registerWriteCallback(getRegNo(SIDRegWvCtl), WvCtl);
        }

        uint8_t const getControl() {
            return WvCtl & 0x0f;
        }

        void setControl(const uint8_t control) {
            WvCtl = (WvCtl & 0xf0) | (control & 0x0f);

            registerWriteCallback(getRegNo(SIDRegWvCtl), WvCtl);
        }

        bool const getTest() {
            return WvCtl & SIDCtlTst;
        }

        void setTest(const bool test) {
            WvCtl = (WvCtl & ~SIDCtlTst) | (test ? SIDCtlTst : 0);

            registerWriteCallback(getRegNo(SIDRegWvCtl), WvCtl);
        }

        bool const getRingMod() {
            return WvCtl & SIDCtlRMd;
        }

        void setRingMod(const bool ringMod) {
            WvCtl = (WvCtl & ~SIDCtlRMd) | (ringMod ? SIDCtlRMd : 0);

            registerWriteCallback(getRegNo(SIDRegWvCtl), WvCtl);
        }

        bool const getSync() {
            return WvCtl & SIDCtlSyn;
        }

        void setSync(const bool sync) {
            WvCtl = (WvCtl & ~SIDCtlSyn) | (sync ? SIDCtlSyn : 0);

            registerWriteCallback(getRegNo(SIDRegWvCtl), WvCtl);
        }

        bool const getGate() {
            return WvCtl & SIDCtlGat;
        }

        void setGate(const bool gate) {
            WvCtl = (WvCtl & ~SIDCtlGat) | (gate ? SIDCtlGat : 0);

            registerWriteCallback(getRegNo(SIDRegWvCtl), WvCtl);
        }

        uint8_t const getAD() {
            return AD;
        }

        void setAD(const uint8_t AD) {
            this->AD = AD;

            registerWriteCallback(getRegNo(SIDRegAD), this->AD);
        }

        uint8_t const getSR() {
            return SR;
        }

        void setSR(const uint8_t SR) {
            this->SR = SR;

            registerWriteCallback(getRegNo(SIDRegSR), this->SR);
        }

        uint16_t const getADSR() {
            return ((uint16_t) AD << 8) | (uint16_t) SR;
        }

        void setADSR(const uint16_t ADSR) {
            AD = (uint8_t) (ADSR >> 8);
            SR = (uint8_t) (ADSR & 0xff);

            registerWriteCallback(getRegNo(SIDRegAD), AD);
            registerWriteCallback(getRegNo(SIDRegSR), SR);
        }

        uint8_t const getAttack() {
            return (AD & 0xf0) >> 4;
        }

        void setAttack(const uint8_t attack) {
            assert(attack <= 0x0f);

            AD = (AD & 0x0f) | (attack << 4);

            registerWriteCallback(getRegNo(SIDRegAD), AD);
        }

        uint8_t const getDecay() {
            return (AD & 0x0f);
        }

        void setDecay(const uint8_t decay) {
            assert(decay <= 0x0f);

            AD = (AD & 0xf0) | decay;

            registerWriteCallback(getRegNo(SIDRegAD), AD);
        }

        uint8_t const getSustain() {
            return (AD & 0xf0) >> 4;
        }

        void setSustain(const uint8_t sustain) {
            assert(sustain <= 0x0f);

            SR = (SR & 0x0f) | (sustain << 4);

            registerWriteCallback(getRegNo(SIDRegSR), SR);
        }

        uint8_t const getRelease() {
            return (SR & 0x0f);
        }

        void setRelease(const uint8_t release) {
            assert(release <= 0x0f);

            SR = (SR & 0xf0) | release;

            registerWriteCallback(getRegNo(SIDRegSR), SR);
        }
    };

    class SIDFilter {
    // filter and volume registers of a SID chip

    public:
        // register numbers
        static const uint8_t SIDRegFCLo    = SID_NUM_VOICES * SID_NUM_VOICE_REGS + 0;
        static const uint8_t SIDRegFCHi    = SID_NUM_VOICES * SID_NUM_VOICE_REGS + 1;
        static const uint8_t SIDRegResFilt = SID_NUM_VOICES * SID_NUM_VOICE_REGS + 2;
        static const uint8_t SIDRegModVol  = SID_NUM_VOICES * SID_NUM_VOICE_REGS + 3;

        static const uint8_t SIDFilt1  = 0x01;
        static const uint8_t SIDFilt2  = 0x02;
        static const uint8_t SIDFilt3  = 0x04;
        static const uint8_t SIDFiltEx = 0x08;

        static const uint8_t SIDFModLP = 0x10;
        static const uint8_t SIDFModBP = 0x20;
        static const uint8_t SIDFModHP = 0x40;

    private:
        const uint8_t SIDNo;
        const uint8_t SIDOffset;

        uint8_t FCLo    = 0; // low byte filter cut-off frequency, only bits 1 to 3 are used
        uint8_t FCHi    = 0; // high byte filter cut-off frequency
        uint8_t ResFilt = 0; // filter resonance and control register
        uint8_t ModVol  = 0; // filter mode and chip volume

        // callback function for register write actions
        std::function<void(const uint8_t, const uint8_t)> registerWriteCallback = [](const uint8_t, const uint8_t) {};

    public:
        SIDFilter(const uint8_t SIDNo) : SIDNo(SIDNo),
                                         SIDOffset(SIDNo << 4) {
        }

        void const setRegisterWriteCallback(std::function<void(const uint8_t, const uint8_t)> cb) {
            registerWriteCallback = cb;
        }

        uint16_t const getFilterFQ() {
            return ((uint16_t) FCLo << 5) | ((uint16_t) FCHi << 8);
        }

        void setFilterFQ(const uint16_t FQ) {
            FCHi = FQ & 0xff00;
            FCLo = (FQ & 0x00ff) >> 5;

            registerWriteCallback(SIDOffset + SIDRegFCHi, FCHi);
            registerWriteCallback(SIDOffset + SIDRegFCLo, FCLo);
        }

        uint16_t const getFilterReq() {
            return ResFilt & 0xf0;
        }

        void setFilterRes(const uint8_t res) {
            ResFilt = (ResFilt & 0x0f) | (res & 0xf0);

            registerWriteCallback(SIDOffset + SIDRegResFilt, ResFilt);
        }

        bool const getFilter1() {
            return ResFilt & SIDFilt1;
        }

        void setFilter1(const bool onOff) {
            ResFilt = (ResFilt & ~SIDFilt1) | (onOff ? SIDFilt1 : 0);

            registerWriteCallback(SIDOffset + SIDRegResFilt, ResFilt);
        }

        bool const getFilter2() {
            return ResFilt & SIDFilt2;
        }

        void setFilter2(const bool onOff) {
            ResFilt = (ResFilt & ~SIDFilt2) | (onOff ? SIDFilt2 : 0);

            registerWriteCallback(SIDOffset + SIDRegResFilt, ResFilt);
        }

        bool const getFilter3() {
            return ResFilt & SIDFilt3;
        }

        void setFilter3(const bool onOff) {
            ResFilt = (ResFilt & ~SIDFilt3) | (onOff ? SIDFilt3 : 0);

            registerWriteCallback(SIDOffset + SIDRegResFilt, ResFilt);
        }

        bool const getFilterEx() {
            return ResFilt & SIDFiltEx;
        }

        void setFilterEx(const bool onOff) {
            ResFilt = (ResFilt & ~SIDFiltEx) | (onOff ? SIDFiltEx : 0);

            registerWriteCallback(SIDOffset + SIDRegResFilt, ResFilt);
        }

        bool const getFilter(const uint8_t voice) {
            assert(voice < 3);

            return ResFilt & (1 << voice);
        }

        void setFilter(const uint8_t voice, const bool onOff) {
            assert(voice < 3);

            uint8_t bit = (1 << voice);
            ResFilt = (ResFilt & ~bit) | (onOff ? bit : 0);

            registerWriteCallback(SIDOffset + SIDRegResFilt, ResFilt);
        }

        uint8_t const getFilterMode() {
            return ModVol & 0x70;
        }

        void setFilterMode(const uint8_t mode) {
            ModVol = (ModVol & 0x0f) | (mode & 0x70);

            registerWriteCallback(SIDOffset + SIDRegModVol, ModVol);
        }

        void setFilterMode(const uint8_t mode, const bool onOff) {
            assert(mode == SIDFModHP || mode == SIDFModBP || mode == SIDFModLP);

            ModVol = (ModVol & ~mode) | (onOff ? mode : 0);

            registerWriteCallback(SIDOffset + SIDRegModVol, ModVol);
        }

        uint8_t const getVolume() {
            return ModVol & 0x0f;
        }

        void setVolume(const uint8_t volume) {
            assert(volume <= 0x0f);

            ModVol = (ModVol & 0xf0) | volume;

            registerWriteCallback(SIDOffset + SIDRegModVol, ModVol);
        }
    };

    class SIDMisc {
    // read only registers of a SID chip, not implemented

    private:
        const uint8_t SIDNo;

        uint8_t PotX = 0;
        uint8_t PotY = 0;
        uint8_t Osc3 = 0;
        uint8_t Env3 = 0;

    public:
        SIDMisc(const uint8_t SIDNo) : SIDNo(SIDNo) {
        }

        uint8_t const getPotX() {
            return PotX;
        }

        uint8_t const getPotY() {
            return PotY;
        }

        uint8_t const getOsc3() {
            return Osc3;
        }

        uint8_t const getEnv3() {
            return Env3;
        }
    };

private:
    const uint8_t SIDNo;

    std::array<SIDVoice, SID_NUM_VOICES> voices = { SIDVoice(SIDNo, 0), SIDVoice(SIDNo, 1), SIDVoice(SIDNo, 2) };
    SIDFilter filter = SIDFilter(SIDNo);
    SIDMisc misc = SIDMisc(SIDNo);

public:
    SID(const uint8_t SIDNo) : SIDNo(SIDNo) {
    }

    uint8_t const getSIDNo() {
        return SIDNo;
    }

    SIDVoice& getVoice(const uint8_t voiceNo) {
        assert(voiceNo < SID_NUM_VOICES);

        return voices[voiceNo];
    }

    SIDFilter& getFilter() {
        return filter;
    }

    SIDMisc& getMisc() {
        return misc;
    }
};

// an array of SID chips
// should be conforming to singleton pattern, but WTF - the whole thing is intended to run on an Arduino

class SIDArray {
private:
    // ring buffer for saving register write actions to be processed by Arduino timer
    RingBuffer<std::tuple<uint8_t, uint8_t>, SID_ARRAY_MAX_NUM_SIDS * SID_NUM_WO_REGS> buffer;

    // array of SID chips
    std::array<SID, SID_ARRAY_MAX_NUM_SIDS> SIDs = { 0, 1, 2, 3, 4, 5 };

    static const void ringBufferCallback(RingBuffer<std::tuple<uint8_t, uint8_t>, SID_ARRAY_MAX_NUM_SIDS * SID_NUM_WO_REGS> buffer,
            const int busyWait, const uint8_t reg, const uint8_t val) {
        if(busyWait) {
            while(buffer.full()) {
                std::cout << "busy\n" << std::flush;
                exit(0);
            }
        }

        buffer.put(std::tuple<uint8_t, uint8_t>(reg, val));
    }

public:
    SIDArray(bool busyWait = false) {
        auto cb = std::bind(ringBufferCallback, buffer, busyWait, std::placeholders::_1, std::placeholders::_2);

        for(uint8_t i = 0; i < SID_ARRAY_MAX_NUM_SIDS; i++) {
            SID& sid = getSID(i);

            sid.getFilter().setRegisterWriteCallback(cb);

            for(uint8_t j = 0; j < SID_NUM_VOICES; j++) {
                sid.getVoice(j).setRegisterWriteCallback(cb);
            }
        }
    }

    SID& getSID(uint8_t SIDNo) {
        assert(SIDNo < SID_ARRAY_MAX_NUM_SIDS);

        return SIDs[SIDNo];
    }
};

#endif // ARDUINOSID_SID_H
