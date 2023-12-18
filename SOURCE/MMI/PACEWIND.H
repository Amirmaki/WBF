// OWLCVT 12/23/96 14:35:12
#ifndef __PACEWIND_H
#define __PACEWIND_H
/*
_CLASSDEF(TPacing)
class _OWLCLASS TPacing : public TDialog
{ public:  */
	 extern float TheoProd, MeasProd;
	 float ManProd;
	 extern char PacingMode;
//    TPacing(TFrameWindow * parent,TResId name);
	 void PacingSetupWindow();
//    virtual BOOL CanClose();
	 void Close();
	 void FixePoint();
	 void PacingSendData();
	 void PacingLoadData();
	 void PacingStoreData();
	 int  PacingGetData();
	 void TesteConfig();
//DECLARE_RESPONSE_TABLE( TPacing );
//};
#endif