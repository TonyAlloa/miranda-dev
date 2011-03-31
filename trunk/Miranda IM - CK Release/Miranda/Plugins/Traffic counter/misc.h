// ������� �������� �������� ������ � ������ string �� �������
void ReplaceBrackets(TCHAR *string);

// ������� ��������� ������ � ��������� �������� � ���������� ������, ��������������� ���
// ������ � ������������� �� ������/������/�� �������
void GetFormattedStrings(TCHAR *src, TCHAR *la, TCHAR *ca, TCHAR *ra);

/* ������� ���������� ���������� ���� � ��������� ������ ���������� ����. */
BYTE DaysInMonth(BYTE Month, WORD Year);

// ������� ���������� ���� ������ �� ����
// 7 - ��, 1 - �� � �. �.
BYTE DayOfWeek(BYTE Day, BYTE Month, WORD Year);

/*
���������:
Value - ���������� ����;
Unit - ������� ��������� (0 - �����, 1 -���������, 2 - ���������, 3 - �������������);
Buffer - ����� ������ ��� ������ ����������
*/
void GetFormattedTraffic(DWORD Value, BYTE Unit, TCHAR *Buffer);

void GetDurationFormatM(DWORD Duration, TCHAR *Format, TCHAR *Buffer, WORD Size);

signed short int TimeCompare(SYSTEMTIME st1, SYSTEMTIME st2);