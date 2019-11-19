#pragma once

#ifndef SID_SID_INCLUDED
#define SID_SID_INCLUDED

/******************************************************************************

	Arduino	6581
	=======	====
	A0		A0
	A1		A1
	A2		A2
	A3		A3
	A4		A4

	D2		D0
	D3		D1
	D4		D2
	D5		D3
	D6		D4
	D7		D5
	D8		D6
	D9		D7

	D10
	D11
	D12
	D13		phi/2

	A5
	A6
	A7

	GND		R/W

******************************************************************************/

#include <array>
#include <functional>
#include <cstdint>
#include <cstddef>
#include <cassert>

#include "ringbuffer.h"

// just for compilation test purposes without the Arduino IDE

#ifndef PIN_A0

#define PIN_A0	(14)
#define PIN_A1	(15)
#define PIN_A2	(16)
#define PIN_A3	(17)
#define PIN_A4	(18)
#define PIN_A5	(19)
#define PIN_A6	(20)
#define PIN_A7	(21)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;

#endif // PIN_A0

// Arduino pin numbers for SID pins

const int SID_A0 = A0;
const int SID_A1 = A1;
const int SID_A2 = A2;
const int SID_A3 = A3;
const int SID_A4 = A4;

const int SID_D0 = 0;
const int SID_D1 = 1;
const int SID_D2 = 2;
const int SID_D3 = 3;
const int SID_D4 = 4;
const int SID_D5 = 5;
const int SID_D6 = 6;
const int SID_D7 = 7;

const int SID_PHI_2 = 13;

const int SID_ARRAY_MAX_NUM_SIDS = 6;

const std::array<int, SID_ARRAY_MAX_NUM_SIDS> SID_CS = { A5, A6, A7, 10, 11, 12 };

const int SID_NUM_VOICES = 3;
const int SID_NUM_VOICE_REGS = 6;
const int SID_NUM_FILTER_REGS = 4;
const int SID_NUM_WO_REGS = SID_NUM_VOICES * SID_NUM_VOICE_REGS + SID_NUM_FILTER_REGS;
const int SID_NUM_RO_REGS = 4;
const int SID_NUM_REGS = SID_NUM_WO_REGS + SID_NUM_RO_REGS;

// registers for a single voice of a SID chip

class SIDVoice {
public:
	// register numbers
	const uint8_t SIDRegFQLo = 0;
	const uint8_t SIDRegFQHi = 1;
	const uint8_t SIDRegPWLo = 2;
	const uint8_t SIDRegPWHi = 3;
	const uint8_t SIDRegWvCtl = 4;
	const uint8_t SIDRegAD = 5;
	const uint8_t SIDRegSR = 6;

	// waveform bits

	const uint8_t SIDWavNse = 0x80;
	const uint8_t SIDWavSqu = 0x40;
	const uint8_t SIDWavSaw = 0x20;
	const uint8_t SIDWavTri = 0x10;

	// control bits

	const uint8_t SIDCtlTst = 0x08;
	const uint8_t SIDCtlRMd = 0x04;
	const uint8_t SIDCtlSyn = 0x02;
	const uint8_t SIDCtlGat = 0x01;

private:
	uint8_t voiceNo;

	uint8_t FQLo = 0;
	uint8_t FQHi = 0;
	uint8_t PWLo = 0;
	uint8_t PWHi = 0;
	uint8_t WvCtl = 0;
	uint8_t AD = 0;
	uint8_t SR = 0;

	std::function<void(const uint8_t, const uint8_t)> registerWriteCallback = [](const uint8_t, const uint8_t) {};

public:
	uint8_t const getVoiceNo() {
		return voiceNo;
	}

    const void setVoiceNo(const uint8_t voiceNo) {
		assert(voiceNo < SID_NUM_VOICES);

		this->voiceNo = voiceNo;
	}

    const void setRegisterWriteCallback(const std::function<void(uint8_t, uint8_t)> cb) {
	    registerWriteCallback = cb;
	}

	uint8_t const getRegNo(uint8_t reg) {
		assert(reg < SID_NUM_VOICE_REGS);

		return (voiceNo * SID_NUM_VOICE_REGS) + reg;
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

// filter and volume registers of a SID chip

class SIDFilter {
public:
	// register numbers
	const uint8_t SIDRegFCLo    = SID_NUM_VOICES * SID_NUM_VOICE_REGS + 0;
	const uint8_t SIDRegFCHi    = SID_NUM_VOICES * SID_NUM_VOICE_REGS + 1;
	const uint8_t SIDRegResFilt = SID_NUM_VOICES * SID_NUM_VOICE_REGS + 2;
	const uint8_t SIDRegModVol  = SID_NUM_VOICES * SID_NUM_VOICE_REGS + 3;

	const uint8_t SIDFilt1 = 0x01;
	const uint8_t SIDFilt2 = 0x02;
	const uint8_t SIDFilt3 = 0x04;
	const uint8_t SIDFiltEx = 0x08;

	const uint8_t SIDFModLP = 0x10;
	const uint8_t SIDFModBP = 0x20;
	const uint8_t SIDFModHP = 0x40;

private:
	uint8_t FCLo = 0;
	uint8_t FCHi = 0;
	uint8_t ResFilt = 0;
	uint8_t ModVol = 0;

    std::function<void(const uint8_t, const uint8_t)> registerWriteCallback = [](const uint8_t, const uint8_t) {};

public:
    void const setRegisterWriteCallback(std::function<void(const uint8_t, const uint8_t)> cb) {
        registerWriteCallback = cb;
    }

    uint16_t const getFilterFQ() {
		return ((uint16_t) FCLo << 5) | ((uint16_t) FCHi << 8);
	}

	void setFilterFQ(const uint16_t FQ) {
		FCHi = FQ & 0xff00;
		FCLo = (FQ & 0x00ff) >> 5;

        registerWriteCallback(SIDRegFCHi, FCHi);
        registerWriteCallback(SIDRegFCLo, FCLo);
	}

	uint16_t const getFilterReq() {
		return ResFilt & 0xf0;
	}

	void setFilterRes(const uint8_t res) {
		ResFilt = (ResFilt & 0x0f) | (res & 0xf0);

        registerWriteCallback(SIDRegResFilt, ResFilt);
	}

	bool const getFilter1() {
		return ResFilt & SIDFilt1;
	}

	void setFilter1(const bool filter) {
		ResFilt = (ResFilt & ~SIDFilt1) | (filter ? SIDFilt1 : 0);

        registerWriteCallback(SIDRegResFilt, ResFilt);
	}

	bool const getFilter2() {
		return ResFilt & SIDFilt2;
	}

	void setFilter2(const bool filter) {
		ResFilt = (ResFilt & ~SIDFilt2) | (filter ? SIDFilt2 : 0);

        registerWriteCallback(SIDRegResFilt, ResFilt);
	}

	bool const getFilter3() {
		return ResFilt & SIDFilt3;
	}

	void setFilter3(const bool filter) {
		ResFilt = (ResFilt & ~SIDFilt3) | (filter ? SIDFilt3 : 0);

        registerWriteCallback(SIDRegResFilt, ResFilt);
	}

	bool const getFilterEx() {
		return ResFilt & SIDFiltEx;
	}

	void setFilterEx(const bool filter) {
		ResFilt = (ResFilt & ~SIDFiltEx) | (filter ? SIDFiltEx : 0);

        registerWriteCallback(SIDRegResFilt, ResFilt);
	}

	bool const getFilter(const int voice) {
		assert(voice < 3);

		return ResFilt & (1 << voice);
	}

	void setFilter(const int voice, const bool filter) {
		assert(voice < 3);

		uint8_t bit = (1 << voice);
		ResFilt = (ResFilt & ~bit) | (filter ? bit : 0);

        registerWriteCallback(SIDRegResFilt, ResFilt);
	}

	uint8_t const getFilterMode() {
		return ModVol & 0x70;
	}

	void setFilterMode(const uint8_t mode) {
		ModVol = (ModVol & 0x0f) | (mode & 0x70);

        registerWriteCallback(SIDRegModVol, ModVol);
	}

	void setFilterMode(const uint8_t mode, const bool onOff) {
		assert(mode == SIDFModHP || mode == SIDFModBP || mode == SIDFModLP);

		ModVol = (ModVol & ~mode) | (onOff ? mode : 0);

        registerWriteCallback(SIDRegModVol, ModVol);
	}

	uint8_t const getVolume() {
		return ModVol & 0x0f;
	}

	void setVolume(const uint8_t volume) {
		assert(volume <= 0x0f);

		ModVol = (ModVol & 0xf0) | volume;

        registerWriteCallback(SIDRegModVol, ModVol);
	}
};

// read only registers of a SID chip, not implemented

class SIDMisc {
private:
	uint8_t PotX = 0;
	uint8_t PotY = 0;
	uint8_t Osc3 = 0;
	uint8_t Env3 = 0;

public:
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

class SID {
private:
    uint8_t SIDNo = 0;

    std::array<SIDVoice, SID_NUM_VOICES> voices;
	SIDFilter filter;
	SIDMisc misc;

public:
	SID() {
		for(uint8_t i = 0; i < SID_NUM_VOICES; i++) {
			voices[i].setVoiceNo(i);
		}
	}

    SIDVoice& getVoice(uint8_t voiceNo) {
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

class SIDArray {

};

#endif // SID_SID_INCLUDED
