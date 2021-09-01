int clipValue(int in, int minVal, int maxVal) {
  int clippedWithMax = min(maxVal, in);
  return max(minVal, clippedWithMax);
}


float scale( float originalMin, float originalMax, float newBegin, float
              newEnd, float inputValue) {

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin) {
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0) {
    rangedValue =  (pow(normalizedCurVal, SCALE_CURVE) * NewRange) + newBegin;
  }
  else     // invert the ranges
  {
    rangedValue =  newBegin - (pow(normalizedCurVal, SCALE_CURVE) * NewRange);
  }
  // clips value to min-max
  return clipValue(rangedValue, newBegin, newEnd);
}
