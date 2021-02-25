@rem *** Empty values
reg add HKCU\RegTypes /v REG_NONE_EMPTY /t REG_NONE /f
reg add HKCU\RegTypes /v REG_BINARY_EMPTY /t REG_BINARY /f
reg add HKCU\RegTypes /v REG_DWORD_EMPTY /t REG_DWORD /f /d 0
reg add HKCU\RegTypes /v REG_DWORD_LITTLE_ENDIAN_EMPTY /t REG_DWORD_LITTLE_ENDIAN /f /d 0
reg add HKCU\RegTypes /v REG_DWORD_BIG_ENDIAN_EMPTY /t REG_DWORD_BIG_ENDIAN /f /d 0
reg add HKCU\RegTypes /v REG_QWORD_EMPTY /t REG_QWORD /f /d 0
reg add HKCU\RegTypes /v REG_QWORD_LITTLE_ENDIAN_EMPTY /t REG_QWORD_LITTLE_ENDIAN /f /d 0
reg add HKCU\RegTypes /v REG_QWORD_BIG_ENDIAN_EMPTY /t REG_QWORD_BIG_ENDIAN /f /d 0

reg add HKCU\RegTypes /v REG_SZ_EMPTY /t REG_SZ /f
reg add HKCU\RegTypes /v REG_EXPAND_SZ_EMPTY /t REG_EXPAND_SZ /f
reg add HKCU\RegTypes /v REG_MULTI_SZ_EMPTY /t REG_MULTI_SZ /f

@rem *** Some test values
reg add HKCU\RegTypes /v REG_NONE /t REG_NONE /f /d FakeString
reg add HKCU\RegTypes /v REG_BINARY /t REG_BINARY /f /d 12345678
reg add HKCU\RegTypes /v REG_DWORD /t REG_DWORD /f /d 0x12345678
reg add HKCU\RegTypes /v REG_DWORD_LITTLE_ENDIAN /t REG_DWORD_LITTLE_ENDIAN /f /d 0x12345678
reg add HKCU\RegTypes /v REG_DWORD_BIG_ENDIAN /t REG_DWORD_BIG_ENDIAN /f /d 0x78563412
reg add HKCU\RegTypes /v REG_QWORD /t REG_QWORD /f /d 123456789ABCDEF0
reg add HKCU\RegTypes /v REG_QWORD_LITTLE_ENDIAN /t REG_QWORD_LITTLE_ENDIAN /f /d 123456789ABCDEF0
reg add HKCU\RegTypes /v REG_QWORD_BIG_ENDIAN /t REG_QWORD_BIG_ENDIAN /f /d F0DEBC9A78563412

reg add HKCU\RegTypes /v REG_SZ /t REG_SZ /f /d String
reg add HKCU\RegTypes /v REG_EXPAND_SZ /t REG_EXPAND_SZ /f /d String
reg add HKCU\RegTypes /v REG_MULTI_SZ /t REG_MULTI_SZ /f /d String1\0String2\0String3

@rem *** The following types are special cases that are not supported by reg
@rem reg add HKCU\RegTypes /v REG_LINK /t REG_LINK /f
@rem reg add HKCU\RegTypes /v REG_RESOURCE_LIST /t REG_RESOURCE_LIST /f
@rem reg add HKCU\RegTypes /v REG_FULL_RESOURCE_DESCRIPTOR /t REG_FULL_RESOURCE_DESCRIPTOR /f
@rem reg add HKCU\RegTypes /v REG_RESOURCE_REQUIREMENTS_LIST /t REG_RESOURCE_REQUIREMENTS_LIST /f
