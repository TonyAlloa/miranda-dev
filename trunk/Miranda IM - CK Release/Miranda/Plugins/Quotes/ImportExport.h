#ifndef __F86374E6_713C_4600_85FB_903A5CDF7251_IMPORT_EXPORT_H__
#define __F86374E6_713C_4600_85FB_903A5CDF7251_IMPORT_EXPORT_H__

INT_PTR Quotes_Export(WPARAM wp,LPARAM lp);
INT_PTR Quotes_Import(WPARAM wp,LPARAM lp);

#ifdef TEST_IMPORT_EXPORT
INT_PTR QuotesMenu_ImportAll(WPARAM wp,LPARAM lp);
INT_PTR QuotesMenu_ExportAll(WPARAM wp,LPARAM lp);
#endif
#endif //__F86374E6_713C_4600_85FB_903A5CDF7251_IMPORT_EXPORT_H__
