#pragma once


struct sbiret {
	long error;	// sbi return error value a0
	long value; // sbi return value a1
};
