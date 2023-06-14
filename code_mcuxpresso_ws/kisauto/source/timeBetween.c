#include "timeBetween.h"
#include "sys.h"

uint8_t between(uint32_t val, uint32_t start, uint32_t end){
	if(start < end && (val >= start && val <= end)
		|| start > end && (val >= start || val <= end)){
		return TRUE;
	}
	return FALSE;
}