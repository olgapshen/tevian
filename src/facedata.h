#ifndef _FACE_DATA_H
#define _FACE_DATA_H

struct FaceData {
  double age;
  bool isMale;
  int x, y, height, width;

  FaceData() {
    age = 0;
    isMale = false;
    x = y = height = width = 0;
  }

  void transform(double scale, int dx, int dy, FaceData &data) const {
    data.x = x / scale + dx;
    data.y = y / scale + dy;
    data.height = height / scale;
    data.width = width / scale;
    data.isMale = isMale;
    data.age = age;
  }
};

#endif // _FACE_DATA_H