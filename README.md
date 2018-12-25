# SLIC-superpixel
simple linear iterative cluster algorithm(SLIC) implement with opencv (c++).

during writing the code, https://github.com/PSMM/SLIC-Superpixels helps me a lot, but his code is based on old version of opencv, and my implementation is based on opencv4.0 which use Mat instead of IplImage.

**here are some test pictures**: a horse, a dog and a person
<div align=center><img src="https://github.com/assassint2017/SLIC-superpixel/blob/master/data/result.png"alt="结果展示图"/></div>

and I write an interface to facilitate tuning parameters and display the result
**here is the interface**
<div align=center><img src="https://github.com/assassint2017/SLIC-superpixel/blob/master/data/Qt5.png"alt="结果展示图"/></div>
