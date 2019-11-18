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

const int SID_CS[] = { A5, A6, A7, 10, 11, 12 };

const int SID_ARRAY_MAX_NUM_SIDS = sizeof(SID_CS) / sizeof(SID_CS[0]);

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

	uint8_t FQLo;
	uint8_t FQHi;
	uint8_t PWLo;
	uint8_t PWHi;
	uint8_t WvCtl;
	uint8_t AD;
	uint8_t SR;

public:
	uint8_t getVoiceNo() {
		return voiceNo;
	}

	void setVoiceNo(uint8_t voiceNo) {
		assert(voiceNo < SID_NUM_VOICES);

		this->voiceNo = voiceNo;
	}

	uint8_t getRegNo(uint8_t reg) {
		assert(reg < SID_NUM_VOICE_REGS);

		return (voiceNo * SID_NUM_VOICE_REGS) + reg;
	}

	uint16_t getFQ() {
		return (((uint16_t) FQHi) << 8) | FQLo;
	}

	void setFQ(uint16_t FQ) {
		FQLo = (uint8_t) (FQ & 0xff);
		FQHi = (uint8_t) (FQ >> 8);
	}

	uint16_t getPW() {
		return (((uint16_t) PWHi) << 12) | (((uint16_t) PWLo) << 4);
	}

	void setPW(uint16_t PW) {
		PWLo = (uint8_t) ((PW >> 4) & 0xff);
		PWHi = (uint8_t) (PW >> 12);
	}

	uint8_t getWave() {
		return WvCtl & 0xf0;
	}

	void setWave(uint8_t wave) {
		WvCtl = (WvCtl & 0x0f) | (wave & 0xf0);
	}

	bool getNoise() {
		return WvCtl & SIDWavNse;
	}

	void setNoise(bool onOff) {
		WvCtl = (WvCtl & ~SIDWavNse) | (onOff ? SIDWavNse : 0);
	}

	bool getSquare() {
		return WvCtl & SIDWavSqu;
	}

	void setSquare(bool onOff) {
		WvCtl = (WvCtl & ~SIDWavSqu) | (onOff ? SIDWavSqu : 0);
	}

	bool getSawtooth() {
		return WvCtl & SIDWavSaw;
	}

	void setSawtooth(bool onOff) {
		WvCtl = (WvCtl & ~SIDWavSaw) | (onOff ? SIDWavSaw : 0);
	}

	bool getTriangle() {
		return WvCtl & SIDWavTri;
	}

	void setTriangle(bool onOff) {
		WvCtl = (WvCtl & ~SIDWavTri) | (onOff ? SIDWavTri : 0);
	}

	uint8_t getControl() {
		return WvCtl & 0x0f;
	}

	void setControl(uint8_t control) {
		WvCtl = (WvCtl & 0xf0) | (control & 0x0f);
	}

	bool getTest() {
		return WvCtl & SIDCtlTst;
	}

	void setTest(bool test) {
		WvCtl = (WvCtl & ~SIDCtlTst) | (test ? SIDCtlTst : 0);
	}

	bool getRingMod() {
		return WvCtl & SIDCtlRMd;
	}

	void setRingMod(bool ringMod) {
		WvCtl = (WvCtl & ~SIDCtlRMd) | (ringMod ? SIDCtlRMd : 0);
	}

	bool getSync() {
		return WvCtl & SIDCtlSyn;
	}

	void setSync(bool sync) {
		WvCtl = (WvCtl & ~SIDCtlSyn) | (sync ? SIDCtlSyn : 0);
	}

	bool getGate() {
		return WvCtl & SIDCtlGat;
	}

	void setGate(bool gate) {
		WvCtl = (WvCtl & ~SIDCtlGat) | (gate ? SIDCtlGat : 0);
	}

	uint8_t getAD() {
		return AD;
	}

	void setAD(uint8_t AD) {
		this->AD = AD;
	}

	uint8_t getSR() {
		return SR;
	}

	void setSR(uint8_t SR) {
		this->SR = SR;
	}

	uint16_t getADSR() {
		return ((uint16_t) AD << 8) | (uint16_t) SR;
	}

	void setADSR(uint16_t ADSR) {
		AD = (uint8_t) (ADSR >> 8);
		SR = (uint8_t) (ADSR & 0xff);
	}

	uint8_t getAttack() {
		return (AD & 0xf0) >> 4;
	}

	void setAttack(uint8_t attack) {
		assert(attack <= 0x0f);

		AD = (AD & 0x0f) | (attack << 4);
	}

	uint8_t getDecay() {
		return (AD & 0x0f);
	}

	void setDecay(uint8_t decay) {
		assert(decay <= 0x0f);

		AD = (AD & 0xf0) | decay;
	}

	uint8_t getSustain() {
		return (AD & 0xf0) >> 4;
	}

	void setSustain(uint8_t sustain) {
		assert(sustain <= 0x0f);

		SR = (SR & 0x0f) | (sustain << 4);
	}

	uint8_t getRelease() {
		return (SR & 0x0f);
	}

	void setRelease(uint8_t release) {
		assert(release <= 0x0f);

		SR = (SR & 0xf0) | release;
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
	uint8_t FCLo;
	uint8_t FCHi;
	uint8_t ResFilt;
	uint8_t ModVol;

public:
	uint16_t getFilterFQ() {
		return ((uint16_t) FCLo << 5) | ((uint16_t) FCHi << 8);
	}

	void setFilterFQ(uint16_t FQ) {
		FCHi = FQ & 0xff00;
		FCLo = (FQ & 0x00ff) >> 5;
	}

	uint16_t getFilterReq() {
		return ResFilt & 0xf0;
	}

	void setFilterRes(uint8_t res) {
		ResFilt = (ResFilt & 0x0f) | (res & 0xf0);
	}

	bool getFilter1() {
		return ResFilt & SIDFilt1;
	}

	void setFilter1(bool filter) {
		ResFilt = (ResFilt & ~SIDFilt1) | (filter ? SIDFilt1 : 0);
	}

	bool getFilter2() {
		return ResFilt & SIDFilt2;
	}

	void setFilter2(bool filter) {
		ResFilt = (ResFilt & ~SIDFilt2) | (filter ? SIDFilt2 : 0);
	}

	bool getFilter3() {
		return ResFilt & SIDFilt3;
	}

	void setFilter3(bool filter) {
		ResFilt = (ResFilt & ~SIDFilt3) | (filter ? SIDFilt3 : 0);
	}

	bool getFilterEx() {
		return ResFilt & SIDFiltEx;
	}

	void setFilterEx(bool filter) {
		ResFilt = (ResFilt & ~SIDFiltEx) | (filter ? SIDFiltEx : 0);
	}

	bool getFilter(int voice) {
		assert(voice < 3);

		return ResFilt & (1 << voice);
	}

	void setFilter(int voice, bool filter) {
		assert(voice < 3);

		uint8_t bit = (1 << voice);
		ResFilt = (ResFilt & ~bit) | (filter ? bit : 0);
	}

	uint8_t getFilterMode() {
		return ModVol & 0x70;
	}

	void setFilterMode(uint8_t mode) {
		ModVol = (ModVol & 0x0f) | (mode & 0x70);
	}

	void setFilterMode(uint8_t mode, bool onOff) {
		assert(mode == SIDFModHP || mode == SIDFModBP || mode == SIDFModLP);

		ModVol = (ModVol & ~mode) | (onOff ? mode : 0);
	}

	uint8_t getVolume() {
		return ModVol & 0x0f;
	}

	void setVolume(uint8_t volume) {
		assert(volume <= 0x0f);

		ModVol = (ModVol & 0xf0) | volume;
	}
};

// read only registers of a SID chip, not implemented

class SIDMisc {
private:
	uint8_t PotX;
	uint8_t PotY;
	uint8_t Osc3;
	uint8_t Env3;

public:
	uint8_t getPotX() {
		return PotX;
	}

	uint8_t getPotY() {
		return PotY;
	}

	uint8_t getOsc3() {
		return Osc3;
	}

	uint8_t getEnv3() {
		return Env3;
	}
};

class SID {
private:
	SIDVoice voice[SID_NUM_VOICES];
	SIDFilter filter;
	SIDMisc misc;

public:
	const SIDVoice& getVoice(uint8_t voiceNo) {
		assert(voiceNo < SID_NUM_VOICES);

		return voice[voiceNo];
	}

	const SIDFilter& getFilter() {
		return filter;
	}

	const SIDMisc& gteMisc() {
		return misc;
	}
};

class SIDArray {

};

#endif // SID_SID_INCLUDED
