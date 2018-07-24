Independent JPEG Group's JPEG software release 6b
  with x86 SIMD extension for IJG JPEG library version 1.02
    == README ==
-----------------------------------------------------------

    ** Note **
The accompanying documents related to x86 SIMD extension are written in
Japanese. The English version of these documents is currently unavailable.
I apologize for this inconvenience to international programmers.

Most of the source code of the extension part is written in assembly
language. To compile the source, you need NASM (netwide assembler).
NASM is available from http://nasm.sourceforge.net/ or
http://sourceforge.net/project/showfiles.php?group_id=6208 .

At present, the x86 SIMD extension doesn't support 64-bit mode of
AMD64 (x86_64).

The x86 SIMD extension is an unofficial extension to the IJG JPEG
software. Please do not send any questions about this distribution
to the Independent JPEG Group.

For conditions of distribution and use, see the IJG's README file.
The same conditions apply to this SIMD-extended JPEG software.



�����̃\�t�g��

  JPEG �̃T�|�[�g���C�u�����Ƃ��čL���g���Ă��� Independent JPEG Group's
  JPEG library (libjpeg ���C�u����) �ɁAIntel x86 �n CPU �̎��� SIMD ���߂�
  ���p�����R�[�h(���[�`��)��V���ɒǉ����A�����������������̂ł��B
  MMX �� SSE �Ȃǂ� SIMD ���Z�@�\�𑕔����Ă���v���Z�b�T��œ��삳����ƁA
  �I���W�i���ł� libjpeg ���C�u�����Ɣ�r���� 2�`3 �{���x�̑��x�œ��삵�܂��B
  �܂��ASIMD ���Ɉ˂�Ȃ��������������������{����Ă���ASIMD ���Z�̎g��
  �Ȃ����^CPU�ɂ����Ă��A�I���W�i���łƔ�r���ď\�������x�����ɓ��삵�܂��B

  JPEG ���k�^�W�J�����̍�������ړI�Ƃ��Ă��܂����A���쑬�x�ŗD��ł͂Ȃ��A
  �I���W�i���łƓ����ȏ�̌v�Z���x�������Ƃ��ŗD��ɍl�����R�[�h���̗p
  ���Ă��܂��B���ہADCT���Z�ɕ��������_DCT���g�����ꍇ�A����сA�������
  �T���v�����O��(h1v2)������JPEG�t�@�C����W�J����ꍇ�������ẮA
  �I���W�i���łƂP�r�b�g�����Ȃ����ʂ��o���܂��B��L�̂Q�̗�O�̏ꍇ��
  �I���W�i���ł��͍��掿��(�����x��)����Ă��܂��B

  SIMD �Ή����ɍۂ��ẮA�\�Ȍ���A�I���W�i���ł� libjpeg ���C�u�����Ƃ�
  �݊����������Ȃ��悤�ɍl������Ă��܂��̂ŁA�قƂ�ǂ̏ꍇ�A�I���W�i��
  �ł����̂܂ܒu�������邱�Ƃ��\�ł��B���ɁA���L���C�u�����Ɋւ��Č����΁A
  �ꕔ�̗�O(cygwin �̏ꍇ)�������A����̓I���W�i���łƃo�C�i�����x���ł�
  ��ʌ݊���������܂��̂ŁA���̂܂܃I���W�i���ł�u�������邱�Ƃ��ł��܂��B

  SIMD �Ή�������Ă��镔���́A�ȉ��̂Ƃ���F

  ���k�����F
    �F��ԕϊ�(RGB->YCbCr)  : MMX or SSE2
    �_�E���T���v�����O      : MMX or SSE2
    DCT���ϊ�(�����x����)   : MMX or SSE2
    DCT���ϊ�(��������)     : MMX or SSE2
    DCT���ϊ�(��������)     : 3DNow! or SSE (�������Z��: MMX or SSE2)
    DCT�W���ʎq��(����)     : MMX or SSE2
    DCT�W���ʎq��(��������) : 3DNow! or SSE (�������Z��: MMX or SSE2)

  �W�J�����F
    �F��ԕϊ�(YCbCr->RGB)  : MMX or SSE2
    �A�b�v�T���v�����O      : MMX or SSE2
    DCT�t�ϊ�(�����x����)   : MMX or SSE2
    DCT�t�ϊ�(��������)     : MMX or SSE2
    DCT�t�ϊ�(��������)     : 3DNow! or SSE (�������Z��: MMX or SSE2)
    DCT�t�ϊ�(�k���W�J)     : MMX or SSE2

  ���jSSE2 �ɂ��ẮASIMD �������Z�݂̂𗘗p���Ă��܂��BSIMD �{���x
      ���������_���Z�͗��p���Ă��܂���B�܂��ASSE3 �͎g�p����Ă��܂���B
      ���� JPEG ���C�u�����ɂ����ẮASSE3 ���g�p���Ă����쑬�x�����
      �����݂��قƂ�ǂȂ����߂ŁASSE3 ���T�|�[�g����\��͂���܂���B

  ���̂ق��ɁA�A�Z���u�������DCT���[�`��(��SIMD; ���ϊ��R��^�t�ϊ��S��)
  �ɂ��ASIMD���߂̎g���Ȃ����^CPU�ł��\�������x�̍����������҂ł��܂��B
  ����ɁA�W�J�����ł̃n�t�}���f�R�[�h���[�`���́ASIMD ���Ɉ˂�Ȃ����@��
  ��������������Ă��܂��B


���Ή����Ă���v���b�g�t�H�[��

  Intel x86 CPU �ɌŗL�̋@�\�𗘗p���Ă��܂��̂ŁA�I���W�i���łƂ͈قȂ�A
  Intel x86 CPU ����т��̌݊� CPU ���̗p���Ă���V�X�e���Ɍ����܂��B
  PowerPC �Ȃǂ� Intel x86 �n�ȊO�̃V�X�e���ɂ͑Ή����Ă��܂���B
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ��̓I�ɂ́A80386 �ȍ~�� Intel x86 CPU ����т��̌݊� CPU ���̗p���Ă���
  �n�[�h�E�F�A�ŁA���A32bit�t���b�g�A�h���X���[�h(�ی샂�[�h)���g�p����
  ����v���b�g�t�H�[��(OS)���Ώۂł��B����ɂ́AWin32 (Windows 9x�n/NT�n)
  ��e�� PC-UNIX (linux �� xBSD �t�@�~���Ȃ�) �Ȃǂ��Y�����܂��B�Ȃ��A
  AMD64 (EM64T) ��64bit���[�h���ɂ͑Ή����Ă��܂���B�����ӂ��������B
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

������ SIMD �g���� IJG JPEG library �ŗL�̐���

  �I���W�i���ł� IJG JPEG library �ł́A�R���p�C�����̃I�v�V�����ŁA
  8bit���xJPEG �� 12bit���xJPEG �̗����ɑΉ����܂����A���� SIMD �g���ł�
  8bit���xJPEG �݂̂̑Ή��ŁA12bit���xJPEG �ɂ͑Ή����܂���B�Ƃ͂����A
  12bit���xJPEG �͈�×p�Ȃǂ̓��ꕪ��������Ėw�ǎg���Ă��Ȃ��̂ŁA
  ���͏��Ȃ��Ǝv���܂��B


���g����

  �}�j���A���́A�ȉ��̃t�@�C���ɕ�����Ă��܂��̂ŁA���ۂ̎g�����Ȃǂ�
  ���ẮA��������Q�Ƃ��Ă��������B

    simd_README.ja.txt   - ���̃t�@�C��
    simd_filelist.ja.txt - ���^�t�@�C���̃t�@�C�����X�g
    simd_install.ja.txt  - �R���p�C���̎d��
    simd_internal.ja.txt - SIMD �g�������̏ڍ�
    simd_cdjpeg.ja.txt   - SIMD �� cjpeg/djpeg �ɌŗL�̋@�\�̉��
    simd_changes.ja.txt  - SIMD �g�������̉��ŗ���


���g�p�����E�T�|�[�g

  ���� SIMD �g���� IJG JPEG software �̎g�p�����ɂ��ẮA�I���W�i���ł�
  IJG JPEG software �̎g�p�������K�p����܂��B�ڂ����́A������ README
  �t�@�C��(�p��)�� LEGAL ISSUES �̍����Q�Ƃ��Ă��������B

  ��L�̎g�p�����̓��e�̌J��Ԃ��ɂȂ�܂����A���̃\�t�g�E�F�A�́u�����
  �܂܂Łv�񋟂���Ă�����̂ŁA���ƓI�Ȏg�p�\���A����ѓ���̖ړI��
  �΂���K�����Ȃǂ��܂߁A�����Ȃ�ۏ؂�����܂���B
  �܂��A�����(The Independent JPEG Group)��������(MIYASAKA Masaru)���A
  ���R�̂�������킸�A�{�\�t�g�E�F�A�̎g�p�ɂ���Ĕ��������@���Ȃ鑹�Q��
  ���Ă��A��؂��̐ӔC�𕉂�Ȃ����̂Ƃ��܂��B

  ���� SIMD �g���� IJG JPEG software �́A�I���W�i���J������ IJG �Ƃ͊֌W
  �Ȃ��A�Ǝ��Ɋg�����s�Ȃ������̂ł��B�ł��̂ŁA���� SIMD �g���� IJG JPEG
  software �Ɋւ��鎿����A�I���W�i���J���� (The Independent JPEG Group)
  �ɑ���Ȃ��ł��������B

  ���� SIMD �g���� IJG JPEG software �Ɋւ��ẮA�����Ƃ��ăm�[�T�|�[�g��
  �����Ă��������܂��B���[���Ȃǂł�����Ȃǂ����������܂��Ă��A��ɉ��炩
  �̕ԓ����ł���킯�ł͂���܂���̂ŁA�����m�������������B
  ���ɁA�i�I���W�i���̉p���}�j���A�����܂߁j�����̃}�j���A���ނɉ񓚂�
  �����Ă��鎿���A�g�p�҂̃\�t�g�E�F�A�Z�p�҂Ƃ��Ă̋Z�ʕs���E�o���s����
  �ւ�鎿��A����̗v�̂𓾂Ȃ�����Ȃǂɂ��ẮA�񓚂��������܂���̂ŁA
  �������炸���������������B



           E-Mail Address : alkaid@coral.ocn.ne.jp (�{�� ��/MIYASAKA Masaru)
[EOF]
