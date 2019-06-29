#ifndef _FACE_DATA_H
#define _FACE_DATA_H

struct FaceData {
  double age;
  bool isMale;
  int x0, x1, y0, y1;

  FaceData() {
    age = 0;
    isMale = false;
    x0 = x1 = y0 = y1 = 0;
  }
};

#endif // _FACE_DATA_H