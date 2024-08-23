else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgCmd1") {
    const AcFltCtrlCfgCmd1 *msg = (const AcFltCtrlCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgCmd2") {
    const AcFltCtrlCfgCmd2 *msg = (const AcFltCtrlCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgSts1") {
    const AcFltCtrlCfgSts1 *msg = (const AcFltCtrlCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgSts2") {
    const AcFltCtrlCfgSts2 *msg = (const AcFltCtrlCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdAmbTemp") {
    const AgsCritCmdAmbTemp *msg = (const AgsCritCmdAmbTemp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcI") {
    const AgsCritCmdDcI *msg = (const AgsCritCmdDcI *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcSoc") {
    const AgsCritCmdDcSoc *msg = (const AgsCritCmdDcSoc *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcV") {
    const AgsCritCmdDcV *msg = (const AgsCritCmdDcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdXferAcV") {
    const AgsCritCmdXferAcV *msg = (const AgsCritCmdXferAcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsAmbTemp") {
    const AgsCritStsAmbTemp *msg = (const AgsCritStsAmbTemp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcI") {
    const AgsCritStsDcI *msg = (const AgsCritStsDcI *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcSoc") {
    const AgsCritStsDcSoc *msg = (const AgsCritStsDcSoc *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcV") {
    const AgsCritStsDcV *msg = (const AgsCritStsDcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsXferAcV") {
    const AgsCritStsXferAcV *msg = (const AgsCritStsXferAcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AlarmCmd") {
    const AlarmCmd *msg = (const AlarmCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AlarmSts") {
    const AlarmSts *msg = (const AlarmSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattCmd") {
    const BattCmd *msg = (const BattCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts1") {
    const BattSts1 *msg = (const BattSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts11") {
    const BattSts11 *msg = (const BattSts11 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts2") {
    const BattSts2 *msg = (const BattSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts3") {
    const BattSts3 *msg = (const BattSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts4") {
    const BattSts4 *msg = (const BattSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts6") {
    const BattSts6 *msg = (const BattSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSumm") {
    const BattSumm *msg = (const BattSumm *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgCmd1") {
    const ChgAcFltCfgCmd1 *msg = (const ChgAcFltCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgCmd2") {
    const ChgAcFltCfgCmd2 *msg = (const ChgAcFltCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgSts1") {
    const ChgAcFltCfgSts1 *msg = (const ChgAcFltCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgSts2") {
    const ChgAcFltCfgSts2 *msg = (const ChgAcFltCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts1") {
    const ChgAcSts1 *msg = (const ChgAcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts2") {
    const ChgAcSts2 *msg = (const ChgAcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts3") {
    const ChgAcSts3 *msg = (const ChgAcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts4") {
    const ChgAcSts4 *msg = (const ChgAcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd1") {
    const ChgCfgCmd1 *msg = (const ChgCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd2") {
    const ChgCfgCmd2 *msg = (const ChgCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd3") {
    const ChgCfgCmd3 *msg = (const ChgCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd4") {
    const ChgCfgCmd4 *msg = (const ChgCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts1") {
    const ChgCfgSts1 *msg = (const ChgCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts2") {
    const ChgCfgSts2 *msg = (const ChgCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts3") {
    const ChgCfgSts3 *msg = (const ChgCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts4") {
    const ChgCfgSts4 *msg = (const ChgCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCmd") {
    const ChgCmd *msg = (const ChgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzCfgCmd") {
    const ChgEqlzCfgCmd *msg = (const ChgEqlzCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzCfgSts") {
    const ChgEqlzCfgSts *msg = (const ChgEqlzCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzSts") {
    const ChgEqlzSts *msg = (const ChgEqlzSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgStatsSts") {
    const ChgStatsSts *msg = (const ChgStatsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts") {
    const ChgSts *msg = (const ChgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts2") {
    const ChgSts2 *msg = (const ChgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts3") {
    const ChgSts3 *msg = (const ChgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts1") {
    const CommSts1 *msg = (const CommSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts2") {
    const CommSts2 *msg = (const CommSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts3") {
    const CommSts3 *msg = (const CommSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcCfgCmd3") {
    const DcSrcCfgCmd3 *msg = (const DcSrcCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcCmd") {
    const DcSrcCmd *msg = (const DcSrcCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcConnSts") {
    const DcSrcConnSts *msg = (const DcSrcConnSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts1") {
    const DcSrcSts1 *msg = (const DcSrcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts11") {
    const DcSrcSts11 *msg = (const DcSrcSts11 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts2") {
    const DcSrcSts2 *msg = (const DcSrcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts3") {
    const DcSrcSts3 *msg = (const DcSrcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts4") {
    const DcSrcSts4 *msg = (const DcSrcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts6") {
    const DcSrcSts6 *msg = (const DcSrcSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DeviceStateSync") {
    const DeviceStateSync *msg = (const DeviceStateSync *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DiagMsg1") {
    const DiagMsg1 *msg = (const DiagMsg1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldBlk") {
    const FileDnldBlk *msg = (const FileDnldBlk *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldInf") {
    const FileDnldInf *msg = (const FileDnldInf *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldSts") {
    const FileDnldSts *msg = (const FileDnldSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenCmd") {
    const GenCmd *msg = (const GenCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenDemandCmd") {
    const GenDemandCmd *msg = (const GenDemandCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenDemandSts") {
    const GenDemandSts *msg = (const GenDemandSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenRst") {
    const GenRst *msg = (const GenRst *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenStartCfgCmd") {
    const GenStartCfgCmd *msg = (const GenStartCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenStartCfgSts") {
    const GenStartCfgSts *msg = (const GenStartCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenSts1") {
    const GenSts1 *msg = (const GenSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenSts2") {
    const GenSts2 *msg = (const GenSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InstAssign") {
    const InstAssign *msg = (const InstAssign *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InstSts") {
    const InstSts *msg = (const InstSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcFltCfgCmd1") {
    const InvAcFltCfgCmd1 *msg = (const InvAcFltCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcFltCfgSts1") {
    const InvAcFltCfgSts1 *msg = (const InvAcFltCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts1") {
    const InvAcSts1 *msg = (const InvAcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts2") {
    const InvAcSts2 *msg = (const InvAcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts3") {
    const InvAcSts3 *msg = (const InvAcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts4") {
    const InvAcSts4 *msg = (const InvAcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvApsSts") {
    const InvApsSts *msg = (const InvApsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd1") {
    const InvCfgCmd1 *msg = (const InvCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd2") {
    const InvCfgCmd2 *msg = (const InvCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd3") {
    const InvCfgCmd3 *msg = (const InvCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd4") {
    const InvCfgCmd4 *msg = (const InvCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts1") {
    const InvCfgSts1 *msg = (const InvCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts2") {
    const InvCfgSts2 *msg = (const InvCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts3") {
    const InvCfgSts3 *msg = (const InvCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts4") {
    const InvCfgSts4 *msg = (const InvCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCmd") {
    const InvCmd *msg = (const InvCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvDcSts") {
    const InvDcSts *msg = (const InvDcSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvHvDcBusSts") {
    const InvHvDcBusSts *msg = (const InvHvDcBusSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvOpsSts") {
    const InvOpsSts *msg = (const InvOpsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvStatsSts") {
    const InvStatsSts *msg = (const InvStatsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvSts") {
    const InvSts *msg = (const InvSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvTempSts") {
    const InvTempSts *msg = (const InvTempSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvTempSts2") {
    const InvTempSts2 *msg = (const InvTempSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoAck") {
    const IsoAck *msg = (const IsoAck *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoAddrClaim") {
    const IsoAddrClaim *msg = (const IsoAddrClaim *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoCmdAddr") {
    const IsoCmdAddr *msg = (const IsoCmdAddr *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoReq") {
    const IsoReq *msg = (const IsoReq *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManAbort") {
    const IsoTpConnManAbort *msg = (const IsoTpConnManAbort *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManBam") {
    const IsoTpConnManBam *msg = (const IsoTpConnManBam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManCts") {
    const IsoTpConnManCts *msg = (const IsoTpConnManCts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManEom") {
    const IsoTpConnManEom *msg = (const IsoTpConnManEom *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManRts") {
    const IsoTpConnManRts *msg = (const IsoTpConnManRts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpXfer") {
    const IsoTpXfer *msg = (const IsoTpXfer *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAcStsRms") {
    const PmAcStsRms *msg = (const PmAcStsRms *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAcXferSwSts") {
    const PmAcXferSwSts *msg = (const PmAcXferSwSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAssocCfg") {
    const PmAssocCfg *msg = (const PmAssocCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAssocSts") {
    const PmAssocSts *msg = (const PmAssocSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmChgSts") {
    const PmChgSts *msg = (const PmChgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmDevMdCfg") {
    const PmDevMdCfg *msg = (const PmDevMdCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmDevMdSts") {
    const PmDevMdSts *msg = (const PmDevMdSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmFxccCfgCmd") {
    const PmFxccCfgCmd *msg = (const PmFxccCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmFxccCfgSts") {
    const PmFxccCfgSts *msg = (const PmFxccCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmGetConfigParam") {
    const PmGetConfigParam *msg = (const PmGetConfigParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmGetStatusParam") {
    const PmGetStatusParam *msg = (const PmGetStatusParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvCtrlCmd") {
    const PmInvCtrlCmd *msg = (const PmInvCtrlCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvLdShSts") {
    const PmInvLdShSts *msg = (const PmInvLdShSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvSts") {
    const PmInvSts *msg = (const PmInvSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmLithionicsCmd") {
    const PmLithionicsCmd *msg = (const PmLithionicsCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmLithionicsSts") {
    const PmLithionicsSts *msg = (const PmLithionicsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAssocCfg") {
    const PmParamAssocCfg *msg = (const PmParamAssocCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfg") {
    const PmParamAuxCfg *msg = (const PmParamAuxCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfgTrigDly") {
    const PmParamAuxCfgTrigDly *msg = (const PmParamAuxCfgTrigDly *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfgTrigLvl") {
    const PmParamAuxCfgTrigLvl *msg = (const PmParamAuxCfgTrigLvl *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxSts") {
    const PmParamAuxSts *msg = (const PmParamAuxSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamDevMdCfg") {
    const PmParamDevMdCfg *msg = (const PmParamDevMdCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnNakRsp") {
    const PmPpnNakRsp *msg = (const PmPpnNakRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnReadCmd") {
    const PmPpnReadCmd *msg = (const PmPpnReadCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnReadRsp") {
    const PmPpnReadRsp *msg = (const PmPpnReadRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnSessionCmd") {
    const PmPpnSessionCmd *msg = (const PmPpnSessionCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnSessionRsp") {
    const PmPpnSessionRsp *msg = (const PmPpnSessionRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnWriteCmd") {
    const PmPpnWriteCmd *msg = (const PmPpnWriteCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnWriteRsp") {
    const PmPpnWriteRsp *msg = (const PmPpnWriteRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmReq") {
    const PmReq *msg = (const PmReq *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmRpcCmd") {
    const PmRpcCmd *msg = (const PmRpcCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmSetConfigParam") {
    const PmSetConfigParam *msg = (const PmSetConfigParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmSwVerSts") {
    const PmSwVerSts *msg = (const PmSwVerSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ProdIdent") {
    const ProdIdent *msg = (const ProdIdent *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ProdInfoSts") {
    const ProdInfoSts *msg = (const ProdInfoSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccArraySts") {
    const SccArraySts *msg = (const SccArraySts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccBattSts") {
    const SccBattSts *msg = (const SccBattSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd") {
    const SccCfgCmd *msg = (const SccCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd2") {
    const SccCfgCmd2 *msg = (const SccCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd3") {
    const SccCfgCmd3 *msg = (const SccCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd4") {
    const SccCfgCmd4 *msg = (const SccCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd5") {
    const SccCfgCmd5 *msg = (const SccCfgCmd5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts") {
    const SccCfgSts *msg = (const SccCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts2") {
    const SccCfgSts2 *msg = (const SccCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts3") {
    const SccCfgSts3 *msg = (const SccCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts4") {
    const SccCfgSts4 *msg = (const SccCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts5") {
    const SccCfgSts5 *msg = (const SccCfgSts5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCmd") {
    const SccCmd *msg = (const SccCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzCfgCmd") {
    const SccEqlzCfgCmd *msg = (const SccEqlzCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzCfgSts") {
    const SccEqlzCfgSts *msg = (const SccEqlzCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzSts") {
    const SccEqlzSts *msg = (const SccEqlzSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts") {
    const SccSts *msg = (const SccSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts2") {
    const SccSts2 *msg = (const SccSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts3") {
    const SccSts3 *msg = (const SccSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts4") {
    const SccSts4 *msg = (const SccSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts5") {
    const SccSts5 *msg = (const SccSts5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts6") {
    const SccSts6 *msg = (const SccSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SetSysDateTime") {
    const SetSysDateTime *msg = (const SetSysDateTime *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SimpleCalibCfg") {
    const SimpleCalibCfg *msg = (const SimpleCalibCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SwVerSts") {
    const SwVerSts *msg = (const SwVerSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SysDateTimeSts") {
    const SysDateTimeSts *msg = (const SysDateTimeSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "VirtualTerminal") {
    const VirtualTerminal *msg = (const VirtualTerminal *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "XbResetCmd") {
    const XbResetCmd *msg = (const XbResetCmd *)ptzRecv->pvStruct;
    msg->Print();
}else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgCmd1") {
    const AcFltCtrlCfgCmd1 *msg = (const AcFltCtrlCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgCmd2") {
    const AcFltCtrlCfgCmd2 *msg = (const AcFltCtrlCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgSts1") {
    const AcFltCtrlCfgSts1 *msg = (const AcFltCtrlCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgSts2") {
    const AcFltCtrlCfgSts2 *msg = (const AcFltCtrlCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdAmbTemp") {
    const AgsCritCmdAmbTemp *msg = (const AgsCritCmdAmbTemp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcI") {
    const AgsCritCmdDcI *msg = (const AgsCritCmdDcI *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcSoc") {
    const AgsCritCmdDcSoc *msg = (const AgsCritCmdDcSoc *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcV") {
    const AgsCritCmdDcV *msg = (const AgsCritCmdDcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdXferAcV") {
    const AgsCritCmdXferAcV *msg = (const AgsCritCmdXferAcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsAmbTemp") {
    const AgsCritStsAmbTemp *msg = (const AgsCritStsAmbTemp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcI") {
    const AgsCritStsDcI *msg = (const AgsCritStsDcI *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcSoc") {
    const AgsCritStsDcSoc *msg = (const AgsCritStsDcSoc *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcV") {
    const AgsCritStsDcV *msg = (const AgsCritStsDcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsXferAcV") {
    const AgsCritStsXferAcV *msg = (const AgsCritStsXferAcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AlarmCmd") {
    const AlarmCmd *msg = (const AlarmCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AlarmSts") {
    const AlarmSts *msg = (const AlarmSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattCmd") {
    const BattCmd *msg = (const BattCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts1") {
    const BattSts1 *msg = (const BattSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts11") {
    const BattSts11 *msg = (const BattSts11 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts2") {
    const BattSts2 *msg = (const BattSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts3") {
    const BattSts3 *msg = (const BattSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts4") {
    const BattSts4 *msg = (const BattSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts6") {
    const BattSts6 *msg = (const BattSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSumm") {
    const BattSumm *msg = (const BattSumm *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgCmd1") {
    const ChgAcFltCfgCmd1 *msg = (const ChgAcFltCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgCmd2") {
    const ChgAcFltCfgCmd2 *msg = (const ChgAcFltCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgSts1") {
    const ChgAcFltCfgSts1 *msg = (const ChgAcFltCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgSts2") {
    const ChgAcFltCfgSts2 *msg = (const ChgAcFltCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts1") {
    const ChgAcSts1 *msg = (const ChgAcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts2") {
    const ChgAcSts2 *msg = (const ChgAcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts3") {
    const ChgAcSts3 *msg = (const ChgAcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts4") {
    const ChgAcSts4 *msg = (const ChgAcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd1") {
    const ChgCfgCmd1 *msg = (const ChgCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd2") {
    const ChgCfgCmd2 *msg = (const ChgCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd3") {
    const ChgCfgCmd3 *msg = (const ChgCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd4") {
    const ChgCfgCmd4 *msg = (const ChgCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts1") {
    const ChgCfgSts1 *msg = (const ChgCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts2") {
    const ChgCfgSts2 *msg = (const ChgCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts3") {
    const ChgCfgSts3 *msg = (const ChgCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts4") {
    const ChgCfgSts4 *msg = (const ChgCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCmd") {
    const ChgCmd *msg = (const ChgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzCfgCmd") {
    const ChgEqlzCfgCmd *msg = (const ChgEqlzCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzCfgSts") {
    const ChgEqlzCfgSts *msg = (const ChgEqlzCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzSts") {
    const ChgEqlzSts *msg = (const ChgEqlzSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgStatsSts") {
    const ChgStatsSts *msg = (const ChgStatsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts") {
    const ChgSts *msg = (const ChgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts2") {
    const ChgSts2 *msg = (const ChgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts3") {
    const ChgSts3 *msg = (const ChgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts1") {
    const CommSts1 *msg = (const CommSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts2") {
    const CommSts2 *msg = (const CommSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts3") {
    const CommSts3 *msg = (const CommSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcCfgCmd3") {
    const DcSrcCfgCmd3 *msg = (const DcSrcCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcCmd") {
    const DcSrcCmd *msg = (const DcSrcCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcConnSts") {
    const DcSrcConnSts *msg = (const DcSrcConnSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts1") {
    const DcSrcSts1 *msg = (const DcSrcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts11") {
    const DcSrcSts11 *msg = (const DcSrcSts11 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts2") {
    const DcSrcSts2 *msg = (const DcSrcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts3") {
    const DcSrcSts3 *msg = (const DcSrcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts4") {
    const DcSrcSts4 *msg = (const DcSrcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts6") {
    const DcSrcSts6 *msg = (const DcSrcSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DeviceStateSync") {
    const DeviceStateSync *msg = (const DeviceStateSync *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DiagMsg1") {
    const DiagMsg1 *msg = (const DiagMsg1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldBlk") {
    const FileDnldBlk *msg = (const FileDnldBlk *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldInf") {
    const FileDnldInf *msg = (const FileDnldInf *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldSts") {
    const FileDnldSts *msg = (const FileDnldSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenCmd") {
    const GenCmd *msg = (const GenCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenDemandCmd") {
    const GenDemandCmd *msg = (const GenDemandCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenDemandSts") {
    const GenDemandSts *msg = (const GenDemandSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenRst") {
    const GenRst *msg = (const GenRst *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenStartCfgCmd") {
    const GenStartCfgCmd *msg = (const GenStartCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenStartCfgSts") {
    const GenStartCfgSts *msg = (const GenStartCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenSts1") {
    const GenSts1 *msg = (const GenSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenSts2") {
    const GenSts2 *msg = (const GenSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InstAssign") {
    const InstAssign *msg = (const InstAssign *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InstSts") {
    const InstSts *msg = (const InstSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcFltCfgCmd1") {
    const InvAcFltCfgCmd1 *msg = (const InvAcFltCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcFltCfgSts1") {
    const InvAcFltCfgSts1 *msg = (const InvAcFltCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts1") {
    const InvAcSts1 *msg = (const InvAcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts2") {
    const InvAcSts2 *msg = (const InvAcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts3") {
    const InvAcSts3 *msg = (const InvAcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts4") {
    const InvAcSts4 *msg = (const InvAcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvApsSts") {
    const InvApsSts *msg = (const InvApsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd1") {
    const InvCfgCmd1 *msg = (const InvCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd2") {
    const InvCfgCmd2 *msg = (const InvCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd3") {
    const InvCfgCmd3 *msg = (const InvCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd4") {
    const InvCfgCmd4 *msg = (const InvCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts1") {
    const InvCfgSts1 *msg = (const InvCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts2") {
    const InvCfgSts2 *msg = (const InvCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts3") {
    const InvCfgSts3 *msg = (const InvCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts4") {
    const InvCfgSts4 *msg = (const InvCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCmd") {
    const InvCmd *msg = (const InvCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvDcSts") {
    const InvDcSts *msg = (const InvDcSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvHvDcBusSts") {
    const InvHvDcBusSts *msg = (const InvHvDcBusSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvOpsSts") {
    const InvOpsSts *msg = (const InvOpsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvStatsSts") {
    const InvStatsSts *msg = (const InvStatsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvSts") {
    const InvSts *msg = (const InvSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvTempSts") {
    const InvTempSts *msg = (const InvTempSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvTempSts2") {
    const InvTempSts2 *msg = (const InvTempSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoAck") {
    const IsoAck *msg = (const IsoAck *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoAddrClaim") {
    const IsoAddrClaim *msg = (const IsoAddrClaim *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoCmdAddr") {
    const IsoCmdAddr *msg = (const IsoCmdAddr *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoReq") {
    const IsoReq *msg = (const IsoReq *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManAbort") {
    const IsoTpConnManAbort *msg = (const IsoTpConnManAbort *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManBam") {
    const IsoTpConnManBam *msg = (const IsoTpConnManBam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManCts") {
    const IsoTpConnManCts *msg = (const IsoTpConnManCts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManEom") {
    const IsoTpConnManEom *msg = (const IsoTpConnManEom *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManRts") {
    const IsoTpConnManRts *msg = (const IsoTpConnManRts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpXfer") {
    const IsoTpXfer *msg = (const IsoTpXfer *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAcStsRms") {
    const PmAcStsRms *msg = (const PmAcStsRms *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAcXferSwSts") {
    const PmAcXferSwSts *msg = (const PmAcXferSwSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAssocCfg") {
    const PmAssocCfg *msg = (const PmAssocCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAssocSts") {
    const PmAssocSts *msg = (const PmAssocSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmChgSts") {
    const PmChgSts *msg = (const PmChgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmDevMdCfg") {
    const PmDevMdCfg *msg = (const PmDevMdCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmDevMdSts") {
    const PmDevMdSts *msg = (const PmDevMdSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmFxccCfgCmd") {
    const PmFxccCfgCmd *msg = (const PmFxccCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmFxccCfgSts") {
    const PmFxccCfgSts *msg = (const PmFxccCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmGetConfigParam") {
    const PmGetConfigParam *msg = (const PmGetConfigParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmGetStatusParam") {
    const PmGetStatusParam *msg = (const PmGetStatusParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvCtrlCmd") {
    const PmInvCtrlCmd *msg = (const PmInvCtrlCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvLdShSts") {
    const PmInvLdShSts *msg = (const PmInvLdShSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvSts") {
    const PmInvSts *msg = (const PmInvSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmLithionicsCmd") {
    const PmLithionicsCmd *msg = (const PmLithionicsCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmLithionicsSts") {
    const PmLithionicsSts *msg = (const PmLithionicsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAssocCfg") {
    const PmParamAssocCfg *msg = (const PmParamAssocCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfg") {
    const PmParamAuxCfg *msg = (const PmParamAuxCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfgTrigDly") {
    const PmParamAuxCfgTrigDly *msg = (const PmParamAuxCfgTrigDly *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfgTrigLvl") {
    const PmParamAuxCfgTrigLvl *msg = (const PmParamAuxCfgTrigLvl *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxSts") {
    const PmParamAuxSts *msg = (const PmParamAuxSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamDevMdCfg") {
    const PmParamDevMdCfg *msg = (const PmParamDevMdCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnNakRsp") {
    const PmPpnNakRsp *msg = (const PmPpnNakRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnReadCmd") {
    const PmPpnReadCmd *msg = (const PmPpnReadCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnReadRsp") {
    const PmPpnReadRsp *msg = (const PmPpnReadRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnSessionCmd") {
    const PmPpnSessionCmd *msg = (const PmPpnSessionCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnSessionRsp") {
    const PmPpnSessionRsp *msg = (const PmPpnSessionRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnWriteCmd") {
    const PmPpnWriteCmd *msg = (const PmPpnWriteCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnWriteRsp") {
    const PmPpnWriteRsp *msg = (const PmPpnWriteRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmReq") {
    const PmReq *msg = (const PmReq *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmRpcCmd") {
    const PmRpcCmd *msg = (const PmRpcCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmSetConfigParam") {
    const PmSetConfigParam *msg = (const PmSetConfigParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmSwVerSts") {
    const PmSwVerSts *msg = (const PmSwVerSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ProdIdent") {
    const ProdIdent *msg = (const ProdIdent *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ProdInfoSts") {
    const ProdInfoSts *msg = (const ProdInfoSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccArraySts") {
    const SccArraySts *msg = (const SccArraySts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccBattSts") {
    const SccBattSts *msg = (const SccBattSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd") {
    const SccCfgCmd *msg = (const SccCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd2") {
    const SccCfgCmd2 *msg = (const SccCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd3") {
    const SccCfgCmd3 *msg = (const SccCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd4") {
    const SccCfgCmd4 *msg = (const SccCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd5") {
    const SccCfgCmd5 *msg = (const SccCfgCmd5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts") {
    const SccCfgSts *msg = (const SccCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts2") {
    const SccCfgSts2 *msg = (const SccCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts3") {
    const SccCfgSts3 *msg = (const SccCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts4") {
    const SccCfgSts4 *msg = (const SccCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts5") {
    const SccCfgSts5 *msg = (const SccCfgSts5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCmd") {
    const SccCmd *msg = (const SccCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzCfgCmd") {
    const SccEqlzCfgCmd *msg = (const SccEqlzCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzCfgSts") {
    const SccEqlzCfgSts *msg = (const SccEqlzCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzSts") {
    const SccEqlzSts *msg = (const SccEqlzSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts") {
    const SccSts *msg = (const SccSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts2") {
    const SccSts2 *msg = (const SccSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts3") {
    const SccSts3 *msg = (const SccSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts4") {
    const SccSts4 *msg = (const SccSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts5") {
    const SccSts5 *msg = (const SccSts5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts6") {
    const SccSts6 *msg = (const SccSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SetSysDateTime") {
    const SetSysDateTime *msg = (const SetSysDateTime *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SimpleCalibCfg") {
    const SimpleCalibCfg *msg = (const SimpleCalibCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SwVerSts") {
    const SwVerSts *msg = (const SwVerSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SysDateTimeSts") {
    const SysDateTimeSts *msg = (const SysDateTimeSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "VirtualTerminal") {
    const VirtualTerminal *msg = (const VirtualTerminal *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "XbResetCmd") {
    const XbResetCmd *msg = (const XbResetCmd *)ptzRecv->pvStruct;
    msg->Print();
}else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgCmd1") {
    const AcFltCtrlCfgCmd1 *msg = (const AcFltCtrlCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgCmd2") {
    const AcFltCtrlCfgCmd2 *msg = (const AcFltCtrlCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgSts1") {
    const AcFltCtrlCfgSts1 *msg = (const AcFltCtrlCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgSts2") {
    const AcFltCtrlCfgSts2 *msg = (const AcFltCtrlCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdAmbTemp") {
    const AgsCritCmdAmbTemp *msg = (const AgsCritCmdAmbTemp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcI") {
    const AgsCritCmdDcI *msg = (const AgsCritCmdDcI *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcSoc") {
    const AgsCritCmdDcSoc *msg = (const AgsCritCmdDcSoc *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcV") {
    const AgsCritCmdDcV *msg = (const AgsCritCmdDcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdXferAcV") {
    const AgsCritCmdXferAcV *msg = (const AgsCritCmdXferAcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsAmbTemp") {
    const AgsCritStsAmbTemp *msg = (const AgsCritStsAmbTemp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcI") {
    const AgsCritStsDcI *msg = (const AgsCritStsDcI *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcSoc") {
    const AgsCritStsDcSoc *msg = (const AgsCritStsDcSoc *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcV") {
    const AgsCritStsDcV *msg = (const AgsCritStsDcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsXferAcV") {
    const AgsCritStsXferAcV *msg = (const AgsCritStsXferAcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AlarmCmd") {
    const AlarmCmd *msg = (const AlarmCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AlarmSts") {
    const AlarmSts *msg = (const AlarmSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattCmd") {
    const BattCmd *msg = (const BattCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts1") {
    const BattSts1 *msg = (const BattSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts11") {
    const BattSts11 *msg = (const BattSts11 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts2") {
    const BattSts2 *msg = (const BattSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts3") {
    const BattSts3 *msg = (const BattSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts4") {
    const BattSts4 *msg = (const BattSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts6") {
    const BattSts6 *msg = (const BattSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSumm") {
    const BattSumm *msg = (const BattSumm *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgCmd1") {
    const ChgAcFltCfgCmd1 *msg = (const ChgAcFltCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgCmd2") {
    const ChgAcFltCfgCmd2 *msg = (const ChgAcFltCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgSts1") {
    const ChgAcFltCfgSts1 *msg = (const ChgAcFltCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgSts2") {
    const ChgAcFltCfgSts2 *msg = (const ChgAcFltCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts1") {
    const ChgAcSts1 *msg = (const ChgAcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts2") {
    const ChgAcSts2 *msg = (const ChgAcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts3") {
    const ChgAcSts3 *msg = (const ChgAcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts4") {
    const ChgAcSts4 *msg = (const ChgAcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd1") {
    const ChgCfgCmd1 *msg = (const ChgCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd2") {
    const ChgCfgCmd2 *msg = (const ChgCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd3") {
    const ChgCfgCmd3 *msg = (const ChgCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd4") {
    const ChgCfgCmd4 *msg = (const ChgCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts1") {
    const ChgCfgSts1 *msg = (const ChgCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts2") {
    const ChgCfgSts2 *msg = (const ChgCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts3") {
    const ChgCfgSts3 *msg = (const ChgCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts4") {
    const ChgCfgSts4 *msg = (const ChgCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCmd") {
    const ChgCmd *msg = (const ChgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzCfgCmd") {
    const ChgEqlzCfgCmd *msg = (const ChgEqlzCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzCfgSts") {
    const ChgEqlzCfgSts *msg = (const ChgEqlzCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzSts") {
    const ChgEqlzSts *msg = (const ChgEqlzSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgStatsSts") {
    const ChgStatsSts *msg = (const ChgStatsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts") {
    const ChgSts *msg = (const ChgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts2") {
    const ChgSts2 *msg = (const ChgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts3") {
    const ChgSts3 *msg = (const ChgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts1") {
    const CommSts1 *msg = (const CommSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts2") {
    const CommSts2 *msg = (const CommSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts3") {
    const CommSts3 *msg = (const CommSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcCfgCmd3") {
    const DcSrcCfgCmd3 *msg = (const DcSrcCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcCmd") {
    const DcSrcCmd *msg = (const DcSrcCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcConnSts") {
    const DcSrcConnSts *msg = (const DcSrcConnSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts1") {
    const DcSrcSts1 *msg = (const DcSrcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts11") {
    const DcSrcSts11 *msg = (const DcSrcSts11 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts2") {
    const DcSrcSts2 *msg = (const DcSrcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts3") {
    const DcSrcSts3 *msg = (const DcSrcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts4") {
    const DcSrcSts4 *msg = (const DcSrcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts6") {
    const DcSrcSts6 *msg = (const DcSrcSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DeviceStateSync") {
    const DeviceStateSync *msg = (const DeviceStateSync *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DiagMsg1") {
    const DiagMsg1 *msg = (const DiagMsg1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldBlk") {
    const FileDnldBlk *msg = (const FileDnldBlk *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldInf") {
    const FileDnldInf *msg = (const FileDnldInf *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldSts") {
    const FileDnldSts *msg = (const FileDnldSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenCmd") {
    const GenCmd *msg = (const GenCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenDemandCmd") {
    const GenDemandCmd *msg = (const GenDemandCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenDemandSts") {
    const GenDemandSts *msg = (const GenDemandSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenRst") {
    const GenRst *msg = (const GenRst *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenStartCfgCmd") {
    const GenStartCfgCmd *msg = (const GenStartCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenStartCfgSts") {
    const GenStartCfgSts *msg = (const GenStartCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenSts1") {
    const GenSts1 *msg = (const GenSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenSts2") {
    const GenSts2 *msg = (const GenSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InstAssign") {
    const InstAssign *msg = (const InstAssign *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InstSts") {
    const InstSts *msg = (const InstSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcFltCfgCmd1") {
    const InvAcFltCfgCmd1 *msg = (const InvAcFltCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcFltCfgSts1") {
    const InvAcFltCfgSts1 *msg = (const InvAcFltCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts1") {
    const InvAcSts1 *msg = (const InvAcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts2") {
    const InvAcSts2 *msg = (const InvAcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts3") {
    const InvAcSts3 *msg = (const InvAcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts4") {
    const InvAcSts4 *msg = (const InvAcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvApsSts") {
    const InvApsSts *msg = (const InvApsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd1") {
    const InvCfgCmd1 *msg = (const InvCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd2") {
    const InvCfgCmd2 *msg = (const InvCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd3") {
    const InvCfgCmd3 *msg = (const InvCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd4") {
    const InvCfgCmd4 *msg = (const InvCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts1") {
    const InvCfgSts1 *msg = (const InvCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts2") {
    const InvCfgSts2 *msg = (const InvCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts3") {
    const InvCfgSts3 *msg = (const InvCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts4") {
    const InvCfgSts4 *msg = (const InvCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCmd") {
    const InvCmd *msg = (const InvCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvDcSts") {
    const InvDcSts *msg = (const InvDcSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvHvDcBusSts") {
    const InvHvDcBusSts *msg = (const InvHvDcBusSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvOpsSts") {
    const InvOpsSts *msg = (const InvOpsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvStatsSts") {
    const InvStatsSts *msg = (const InvStatsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvSts") {
    const InvSts *msg = (const InvSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvTempSts") {
    const InvTempSts *msg = (const InvTempSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvTempSts2") {
    const InvTempSts2 *msg = (const InvTempSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoAck") {
    const IsoAck *msg = (const IsoAck *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoAddrClaim") {
    const IsoAddrClaim *msg = (const IsoAddrClaim *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoCmdAddr") {
    const IsoCmdAddr *msg = (const IsoCmdAddr *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoReq") {
    const IsoReq *msg = (const IsoReq *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManAbort") {
    const IsoTpConnManAbort *msg = (const IsoTpConnManAbort *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManBam") {
    const IsoTpConnManBam *msg = (const IsoTpConnManBam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManCts") {
    const IsoTpConnManCts *msg = (const IsoTpConnManCts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManEom") {
    const IsoTpConnManEom *msg = (const IsoTpConnManEom *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManRts") {
    const IsoTpConnManRts *msg = (const IsoTpConnManRts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpXfer") {
    const IsoTpXfer *msg = (const IsoTpXfer *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAcStsRms") {
    const PmAcStsRms *msg = (const PmAcStsRms *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAcXferSwSts") {
    const PmAcXferSwSts *msg = (const PmAcXferSwSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAssocCfg") {
    const PmAssocCfg *msg = (const PmAssocCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAssocSts") {
    const PmAssocSts *msg = (const PmAssocSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmChgSts") {
    const PmChgSts *msg = (const PmChgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmDevMdCfg") {
    const PmDevMdCfg *msg = (const PmDevMdCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmDevMdSts") {
    const PmDevMdSts *msg = (const PmDevMdSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmFxccCfgCmd") {
    const PmFxccCfgCmd *msg = (const PmFxccCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmFxccCfgSts") {
    const PmFxccCfgSts *msg = (const PmFxccCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmGetConfigParam") {
    const PmGetConfigParam *msg = (const PmGetConfigParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmGetStatusParam") {
    const PmGetStatusParam *msg = (const PmGetStatusParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvCtrlCmd") {
    const PmInvCtrlCmd *msg = (const PmInvCtrlCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvLdShSts") {
    const PmInvLdShSts *msg = (const PmInvLdShSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvSts") {
    const PmInvSts *msg = (const PmInvSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmLithionicsCmd") {
    const PmLithionicsCmd *msg = (const PmLithionicsCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmLithionicsSts") {
    const PmLithionicsSts *msg = (const PmLithionicsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAssocCfg") {
    const PmParamAssocCfg *msg = (const PmParamAssocCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfg") {
    const PmParamAuxCfg *msg = (const PmParamAuxCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfgTrigDly") {
    const PmParamAuxCfgTrigDly *msg = (const PmParamAuxCfgTrigDly *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfgTrigLvl") {
    const PmParamAuxCfgTrigLvl *msg = (const PmParamAuxCfgTrigLvl *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxSts") {
    const PmParamAuxSts *msg = (const PmParamAuxSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamDevMdCfg") {
    const PmParamDevMdCfg *msg = (const PmParamDevMdCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnNakRsp") {
    const PmPpnNakRsp *msg = (const PmPpnNakRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnReadCmd") {
    const PmPpnReadCmd *msg = (const PmPpnReadCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnReadRsp") {
    const PmPpnReadRsp *msg = (const PmPpnReadRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnSessionCmd") {
    const PmPpnSessionCmd *msg = (const PmPpnSessionCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnSessionRsp") {
    const PmPpnSessionRsp *msg = (const PmPpnSessionRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnWriteCmd") {
    const PmPpnWriteCmd *msg = (const PmPpnWriteCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnWriteRsp") {
    const PmPpnWriteRsp *msg = (const PmPpnWriteRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmReq") {
    const PmReq *msg = (const PmReq *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmRpcCmd") {
    const PmRpcCmd *msg = (const PmRpcCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmSetConfigParam") {
    const PmSetConfigParam *msg = (const PmSetConfigParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmSwVerSts") {
    const PmSwVerSts *msg = (const PmSwVerSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ProdIdent") {
    const ProdIdent *msg = (const ProdIdent *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ProdInfoSts") {
    const ProdInfoSts *msg = (const ProdInfoSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccArraySts") {
    const SccArraySts *msg = (const SccArraySts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccBattSts") {
    const SccBattSts *msg = (const SccBattSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd") {
    const SccCfgCmd *msg = (const SccCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd2") {
    const SccCfgCmd2 *msg = (const SccCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd3") {
    const SccCfgCmd3 *msg = (const SccCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd4") {
    const SccCfgCmd4 *msg = (const SccCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd5") {
    const SccCfgCmd5 *msg = (const SccCfgCmd5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts") {
    const SccCfgSts *msg = (const SccCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts2") {
    const SccCfgSts2 *msg = (const SccCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts3") {
    const SccCfgSts3 *msg = (const SccCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts4") {
    const SccCfgSts4 *msg = (const SccCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts5") {
    const SccCfgSts5 *msg = (const SccCfgSts5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCmd") {
    const SccCmd *msg = (const SccCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzCfgCmd") {
    const SccEqlzCfgCmd *msg = (const SccEqlzCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzCfgSts") {
    const SccEqlzCfgSts *msg = (const SccEqlzCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzSts") {
    const SccEqlzSts *msg = (const SccEqlzSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts") {
    const SccSts *msg = (const SccSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts2") {
    const SccSts2 *msg = (const SccSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts3") {
    const SccSts3 *msg = (const SccSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts4") {
    const SccSts4 *msg = (const SccSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts5") {
    const SccSts5 *msg = (const SccSts5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts6") {
    const SccSts6 *msg = (const SccSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SetSysDateTime") {
    const SetSysDateTime *msg = (const SetSysDateTime *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SimpleCalibCfg") {
    const SimpleCalibCfg *msg = (const SimpleCalibCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SwVerSts") {
    const SwVerSts *msg = (const SwVerSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SysDateTimeSts") {
    const SysDateTimeSts *msg = (const SysDateTimeSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "VirtualTerminal") {
    const VirtualTerminal *msg = (const VirtualTerminal *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "XbResetCmd") {
    const XbResetCmd *msg = (const XbResetCmd *)ptzRecv->pvStruct;
    msg->Print();
}else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgCmd1") {
    const AcFltCtrlCfgCmd1 *msg = (const AcFltCtrlCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgCmd2") {
    const AcFltCtrlCfgCmd2 *msg = (const AcFltCtrlCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgSts1") {
    const AcFltCtrlCfgSts1 *msg = (const AcFltCtrlCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgSts2") {
    const AcFltCtrlCfgSts2 *msg = (const AcFltCtrlCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdAmbTemp") {
    const AgsCritCmdAmbTemp *msg = (const AgsCritCmdAmbTemp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcI") {
    const AgsCritCmdDcI *msg = (const AgsCritCmdDcI *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcSoc") {
    const AgsCritCmdDcSoc *msg = (const AgsCritCmdDcSoc *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcV") {
    const AgsCritCmdDcV *msg = (const AgsCritCmdDcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdXferAcV") {
    const AgsCritCmdXferAcV *msg = (const AgsCritCmdXferAcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsAmbTemp") {
    const AgsCritStsAmbTemp *msg = (const AgsCritStsAmbTemp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcI") {
    const AgsCritStsDcI *msg = (const AgsCritStsDcI *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcSoc") {
    const AgsCritStsDcSoc *msg = (const AgsCritStsDcSoc *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcV") {
    const AgsCritStsDcV *msg = (const AgsCritStsDcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsXferAcV") {
    const AgsCritStsXferAcV *msg = (const AgsCritStsXferAcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AlarmCmd") {
    const AlarmCmd *msg = (const AlarmCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AlarmSts") {
    const AlarmSts *msg = (const AlarmSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattCmd") {
    const BattCmd *msg = (const BattCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts1") {
    const BattSts1 *msg = (const BattSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts11") {
    const BattSts11 *msg = (const BattSts11 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts2") {
    const BattSts2 *msg = (const BattSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts3") {
    const BattSts3 *msg = (const BattSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts4") {
    const BattSts4 *msg = (const BattSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts6") {
    const BattSts6 *msg = (const BattSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSumm") {
    const BattSumm *msg = (const BattSumm *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgCmd1") {
    const ChgAcFltCfgCmd1 *msg = (const ChgAcFltCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgCmd2") {
    const ChgAcFltCfgCmd2 *msg = (const ChgAcFltCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgSts1") {
    const ChgAcFltCfgSts1 *msg = (const ChgAcFltCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgSts2") {
    const ChgAcFltCfgSts2 *msg = (const ChgAcFltCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts1") {
    const ChgAcSts1 *msg = (const ChgAcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts2") {
    const ChgAcSts2 *msg = (const ChgAcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts3") {
    const ChgAcSts3 *msg = (const ChgAcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts4") {
    const ChgAcSts4 *msg = (const ChgAcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd1") {
    const ChgCfgCmd1 *msg = (const ChgCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd2") {
    const ChgCfgCmd2 *msg = (const ChgCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd3") {
    const ChgCfgCmd3 *msg = (const ChgCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd4") {
    const ChgCfgCmd4 *msg = (const ChgCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts1") {
    const ChgCfgSts1 *msg = (const ChgCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts2") {
    const ChgCfgSts2 *msg = (const ChgCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts3") {
    const ChgCfgSts3 *msg = (const ChgCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts4") {
    const ChgCfgSts4 *msg = (const ChgCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCmd") {
    const ChgCmd *msg = (const ChgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzCfgCmd") {
    const ChgEqlzCfgCmd *msg = (const ChgEqlzCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzCfgSts") {
    const ChgEqlzCfgSts *msg = (const ChgEqlzCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzSts") {
    const ChgEqlzSts *msg = (const ChgEqlzSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgStatsSts") {
    const ChgStatsSts *msg = (const ChgStatsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts") {
    const ChgSts *msg = (const ChgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts2") {
    const ChgSts2 *msg = (const ChgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts3") {
    const ChgSts3 *msg = (const ChgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts1") {
    const CommSts1 *msg = (const CommSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts2") {
    const CommSts2 *msg = (const CommSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts3") {
    const CommSts3 *msg = (const CommSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcCfgCmd3") {
    const DcSrcCfgCmd3 *msg = (const DcSrcCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcCmd") {
    const DcSrcCmd *msg = (const DcSrcCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcConnSts") {
    const DcSrcConnSts *msg = (const DcSrcConnSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts1") {
    const DcSrcSts1 *msg = (const DcSrcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts11") {
    const DcSrcSts11 *msg = (const DcSrcSts11 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts2") {
    const DcSrcSts2 *msg = (const DcSrcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts3") {
    const DcSrcSts3 *msg = (const DcSrcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts4") {
    const DcSrcSts4 *msg = (const DcSrcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts6") {
    const DcSrcSts6 *msg = (const DcSrcSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DeviceStateSync") {
    const DeviceStateSync *msg = (const DeviceStateSync *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DiagMsg1") {
    const DiagMsg1 *msg = (const DiagMsg1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldBlk") {
    const FileDnldBlk *msg = (const FileDnldBlk *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldInf") {
    const FileDnldInf *msg = (const FileDnldInf *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldSts") {
    const FileDnldSts *msg = (const FileDnldSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenCmd") {
    const GenCmd *msg = (const GenCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenDemandCmd") {
    const GenDemandCmd *msg = (const GenDemandCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenDemandSts") {
    const GenDemandSts *msg = (const GenDemandSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenRst") {
    const GenRst *msg = (const GenRst *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenStartCfgCmd") {
    const GenStartCfgCmd *msg = (const GenStartCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenStartCfgSts") {
    const GenStartCfgSts *msg = (const GenStartCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenSts1") {
    const GenSts1 *msg = (const GenSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenSts2") {
    const GenSts2 *msg = (const GenSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InstAssign") {
    const InstAssign *msg = (const InstAssign *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InstSts") {
    const InstSts *msg = (const InstSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcFltCfgCmd1") {
    const InvAcFltCfgCmd1 *msg = (const InvAcFltCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcFltCfgSts1") {
    const InvAcFltCfgSts1 *msg = (const InvAcFltCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts1") {
    const InvAcSts1 *msg = (const InvAcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts2") {
    const InvAcSts2 *msg = (const InvAcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts3") {
    const InvAcSts3 *msg = (const InvAcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts4") {
    const InvAcSts4 *msg = (const InvAcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvApsSts") {
    const InvApsSts *msg = (const InvApsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd1") {
    const InvCfgCmd1 *msg = (const InvCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd2") {
    const InvCfgCmd2 *msg = (const InvCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd3") {
    const InvCfgCmd3 *msg = (const InvCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd4") {
    const InvCfgCmd4 *msg = (const InvCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts1") {
    const InvCfgSts1 *msg = (const InvCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts2") {
    const InvCfgSts2 *msg = (const InvCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts3") {
    const InvCfgSts3 *msg = (const InvCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts4") {
    const InvCfgSts4 *msg = (const InvCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCmd") {
    const InvCmd *msg = (const InvCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvDcSts") {
    const InvDcSts *msg = (const InvDcSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvHvDcBusSts") {
    const InvHvDcBusSts *msg = (const InvHvDcBusSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvOpsSts") {
    const InvOpsSts *msg = (const InvOpsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvStatsSts") {
    const InvStatsSts *msg = (const InvStatsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvSts") {
    const InvSts *msg = (const InvSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvTempSts") {
    const InvTempSts *msg = (const InvTempSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvTempSts2") {
    const InvTempSts2 *msg = (const InvTempSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoAck") {
    const IsoAck *msg = (const IsoAck *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoAddrClaim") {
    const IsoAddrClaim *msg = (const IsoAddrClaim *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoCmdAddr") {
    const IsoCmdAddr *msg = (const IsoCmdAddr *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoReq") {
    const IsoReq *msg = (const IsoReq *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManAbort") {
    const IsoTpConnManAbort *msg = (const IsoTpConnManAbort *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManBam") {
    const IsoTpConnManBam *msg = (const IsoTpConnManBam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManCts") {
    const IsoTpConnManCts *msg = (const IsoTpConnManCts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManEom") {
    const IsoTpConnManEom *msg = (const IsoTpConnManEom *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManRts") {
    const IsoTpConnManRts *msg = (const IsoTpConnManRts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpXfer") {
    const IsoTpXfer *msg = (const IsoTpXfer *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAcStsRms") {
    const PmAcStsRms *msg = (const PmAcStsRms *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAcXferSwSts") {
    const PmAcXferSwSts *msg = (const PmAcXferSwSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAssocCfg") {
    const PmAssocCfg *msg = (const PmAssocCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAssocSts") {
    const PmAssocSts *msg = (const PmAssocSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmChgSts") {
    const PmChgSts *msg = (const PmChgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmDevMdCfg") {
    const PmDevMdCfg *msg = (const PmDevMdCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmDevMdSts") {
    const PmDevMdSts *msg = (const PmDevMdSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmFxccCfgCmd") {
    const PmFxccCfgCmd *msg = (const PmFxccCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmFxccCfgSts") {
    const PmFxccCfgSts *msg = (const PmFxccCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmGetConfigParam") {
    const PmGetConfigParam *msg = (const PmGetConfigParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmGetStatusParam") {
    const PmGetStatusParam *msg = (const PmGetStatusParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvCtrlCmd") {
    const PmInvCtrlCmd *msg = (const PmInvCtrlCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvLdShSts") {
    const PmInvLdShSts *msg = (const PmInvLdShSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvSts") {
    const PmInvSts *msg = (const PmInvSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmLithionicsCmd") {
    const PmLithionicsCmd *msg = (const PmLithionicsCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmLithionicsSts") {
    const PmLithionicsSts *msg = (const PmLithionicsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAssocCfg") {
    const PmParamAssocCfg *msg = (const PmParamAssocCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfg") {
    const PmParamAuxCfg *msg = (const PmParamAuxCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfgTrigDly") {
    const PmParamAuxCfgTrigDly *msg = (const PmParamAuxCfgTrigDly *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfgTrigLvl") {
    const PmParamAuxCfgTrigLvl *msg = (const PmParamAuxCfgTrigLvl *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxSts") {
    const PmParamAuxSts *msg = (const PmParamAuxSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamDevMdCfg") {
    const PmParamDevMdCfg *msg = (const PmParamDevMdCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnNakRsp") {
    const PmPpnNakRsp *msg = (const PmPpnNakRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnReadCmd") {
    const PmPpnReadCmd *msg = (const PmPpnReadCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnReadRsp") {
    const PmPpnReadRsp *msg = (const PmPpnReadRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnSessionCmd") {
    const PmPpnSessionCmd *msg = (const PmPpnSessionCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnSessionRsp") {
    const PmPpnSessionRsp *msg = (const PmPpnSessionRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnWriteCmd") {
    const PmPpnWriteCmd *msg = (const PmPpnWriteCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnWriteRsp") {
    const PmPpnWriteRsp *msg = (const PmPpnWriteRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmReq") {
    const PmReq *msg = (const PmReq *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmRpcCmd") {
    const PmRpcCmd *msg = (const PmRpcCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmSetConfigParam") {
    const PmSetConfigParam *msg = (const PmSetConfigParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmSwVerSts") {
    const PmSwVerSts *msg = (const PmSwVerSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ProdIdent") {
    const ProdIdent *msg = (const ProdIdent *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ProdInfoSts") {
    const ProdInfoSts *msg = (const ProdInfoSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccArraySts") {
    const SccArraySts *msg = (const SccArraySts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccBattSts") {
    const SccBattSts *msg = (const SccBattSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd") {
    const SccCfgCmd *msg = (const SccCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd2") {
    const SccCfgCmd2 *msg = (const SccCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd3") {
    const SccCfgCmd3 *msg = (const SccCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd4") {
    const SccCfgCmd4 *msg = (const SccCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd5") {
    const SccCfgCmd5 *msg = (const SccCfgCmd5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts") {
    const SccCfgSts *msg = (const SccCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts2") {
    const SccCfgSts2 *msg = (const SccCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts3") {
    const SccCfgSts3 *msg = (const SccCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts4") {
    const SccCfgSts4 *msg = (const SccCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts5") {
    const SccCfgSts5 *msg = (const SccCfgSts5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCmd") {
    const SccCmd *msg = (const SccCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzCfgCmd") {
    const SccEqlzCfgCmd *msg = (const SccEqlzCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzCfgSts") {
    const SccEqlzCfgSts *msg = (const SccEqlzCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzSts") {
    const SccEqlzSts *msg = (const SccEqlzSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts") {
    const SccSts *msg = (const SccSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts2") {
    const SccSts2 *msg = (const SccSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts3") {
    const SccSts3 *msg = (const SccSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts4") {
    const SccSts4 *msg = (const SccSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts5") {
    const SccSts5 *msg = (const SccSts5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts6") {
    const SccSts6 *msg = (const SccSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SetSysDateTime") {
    const SetSysDateTime *msg = (const SetSysDateTime *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SimpleCalibCfg") {
    const SimpleCalibCfg *msg = (const SimpleCalibCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SwVerSts") {
    const SwVerSts *msg = (const SwVerSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SysDateTimeSts") {
    const SysDateTimeSts *msg = (const SysDateTimeSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "VirtualTerminal") {
    const VirtualTerminal *msg = (const VirtualTerminal *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "XbResetCmd") {
    const XbResetCmd *msg = (const XbResetCmd *)ptzRecv->pvStruct;
    msg->Print();
}else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgCmd1") {
    const AcFltCtrlCfgCmd1 *msg = (const AcFltCtrlCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgCmd2") {
    const AcFltCtrlCfgCmd2 *msg = (const AcFltCtrlCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgSts1") {
    const AcFltCtrlCfgSts1 *msg = (const AcFltCtrlCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgSts2") {
    const AcFltCtrlCfgSts2 *msg = (const AcFltCtrlCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdAmbTemp") {
    const AgsCritCmdAmbTemp *msg = (const AgsCritCmdAmbTemp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcI") {
    const AgsCritCmdDcI *msg = (const AgsCritCmdDcI *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcSoc") {
    const AgsCritCmdDcSoc *msg = (const AgsCritCmdDcSoc *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcV") {
    const AgsCritCmdDcV *msg = (const AgsCritCmdDcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdXferAcV") {
    const AgsCritCmdXferAcV *msg = (const AgsCritCmdXferAcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsAmbTemp") {
    const AgsCritStsAmbTemp *msg = (const AgsCritStsAmbTemp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcI") {
    const AgsCritStsDcI *msg = (const AgsCritStsDcI *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcSoc") {
    const AgsCritStsDcSoc *msg = (const AgsCritStsDcSoc *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcV") {
    const AgsCritStsDcV *msg = (const AgsCritStsDcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsXferAcV") {
    const AgsCritStsXferAcV *msg = (const AgsCritStsXferAcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AlarmCmd") {
    const AlarmCmd *msg = (const AlarmCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AlarmSts") {
    const AlarmSts *msg = (const AlarmSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattCmd") {
    const BattCmd *msg = (const BattCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts1") {
    const BattSts1 *msg = (const BattSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts11") {
    const BattSts11 *msg = (const BattSts11 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts2") {
    const BattSts2 *msg = (const BattSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts3") {
    const BattSts3 *msg = (const BattSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts4") {
    const BattSts4 *msg = (const BattSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts6") {
    const BattSts6 *msg = (const BattSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSumm") {
    const BattSumm *msg = (const BattSumm *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgCmd1") {
    const ChgAcFltCfgCmd1 *msg = (const ChgAcFltCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgCmd2") {
    const ChgAcFltCfgCmd2 *msg = (const ChgAcFltCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgSts1") {
    const ChgAcFltCfgSts1 *msg = (const ChgAcFltCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgSts2") {
    const ChgAcFltCfgSts2 *msg = (const ChgAcFltCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts1") {
    const ChgAcSts1 *msg = (const ChgAcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts2") {
    const ChgAcSts2 *msg = (const ChgAcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts3") {
    const ChgAcSts3 *msg = (const ChgAcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts4") {
    const ChgAcSts4 *msg = (const ChgAcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd1") {
    const ChgCfgCmd1 *msg = (const ChgCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd2") {
    const ChgCfgCmd2 *msg = (const ChgCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd3") {
    const ChgCfgCmd3 *msg = (const ChgCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd4") {
    const ChgCfgCmd4 *msg = (const ChgCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts1") {
    const ChgCfgSts1 *msg = (const ChgCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts2") {
    const ChgCfgSts2 *msg = (const ChgCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts3") {
    const ChgCfgSts3 *msg = (const ChgCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts4") {
    const ChgCfgSts4 *msg = (const ChgCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCmd") {
    const ChgCmd *msg = (const ChgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzCfgCmd") {
    const ChgEqlzCfgCmd *msg = (const ChgEqlzCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzCfgSts") {
    const ChgEqlzCfgSts *msg = (const ChgEqlzCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzSts") {
    const ChgEqlzSts *msg = (const ChgEqlzSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgStatsSts") {
    const ChgStatsSts *msg = (const ChgStatsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts") {
    const ChgSts *msg = (const ChgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts2") {
    const ChgSts2 *msg = (const ChgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts3") {
    const ChgSts3 *msg = (const ChgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts1") {
    const CommSts1 *msg = (const CommSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts2") {
    const CommSts2 *msg = (const CommSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts3") {
    const CommSts3 *msg = (const CommSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcCfgCmd3") {
    const DcSrcCfgCmd3 *msg = (const DcSrcCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcCmd") {
    const DcSrcCmd *msg = (const DcSrcCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcConnSts") {
    const DcSrcConnSts *msg = (const DcSrcConnSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts1") {
    const DcSrcSts1 *msg = (const DcSrcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts11") {
    const DcSrcSts11 *msg = (const DcSrcSts11 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts2") {
    const DcSrcSts2 *msg = (const DcSrcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts3") {
    const DcSrcSts3 *msg = (const DcSrcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts4") {
    const DcSrcSts4 *msg = (const DcSrcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts6") {
    const DcSrcSts6 *msg = (const DcSrcSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DeviceStateSync") {
    const DeviceStateSync *msg = (const DeviceStateSync *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DiagMsg1") {
    const DiagMsg1 *msg = (const DiagMsg1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldBlk") {
    const FileDnldBlk *msg = (const FileDnldBlk *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldInf") {
    const FileDnldInf *msg = (const FileDnldInf *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldSts") {
    const FileDnldSts *msg = (const FileDnldSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenCmd") {
    const GenCmd *msg = (const GenCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenDemandCmd") {
    const GenDemandCmd *msg = (const GenDemandCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenDemandSts") {
    const GenDemandSts *msg = (const GenDemandSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenRst") {
    const GenRst *msg = (const GenRst *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenStartCfgCmd") {
    const GenStartCfgCmd *msg = (const GenStartCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenStartCfgSts") {
    const GenStartCfgSts *msg = (const GenStartCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenSts1") {
    const GenSts1 *msg = (const GenSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenSts2") {
    const GenSts2 *msg = (const GenSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InstAssign") {
    const InstAssign *msg = (const InstAssign *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InstSts") {
    const InstSts *msg = (const InstSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcFltCfgCmd1") {
    const InvAcFltCfgCmd1 *msg = (const InvAcFltCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcFltCfgSts1") {
    const InvAcFltCfgSts1 *msg = (const InvAcFltCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts1") {
    const InvAcSts1 *msg = (const InvAcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts2") {
    const InvAcSts2 *msg = (const InvAcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts3") {
    const InvAcSts3 *msg = (const InvAcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts4") {
    const InvAcSts4 *msg = (const InvAcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvApsSts") {
    const InvApsSts *msg = (const InvApsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd1") {
    const InvCfgCmd1 *msg = (const InvCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd2") {
    const InvCfgCmd2 *msg = (const InvCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd3") {
    const InvCfgCmd3 *msg = (const InvCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd4") {
    const InvCfgCmd4 *msg = (const InvCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts1") {
    const InvCfgSts1 *msg = (const InvCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts2") {
    const InvCfgSts2 *msg = (const InvCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts3") {
    const InvCfgSts3 *msg = (const InvCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts4") {
    const InvCfgSts4 *msg = (const InvCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCmd") {
    const InvCmd *msg = (const InvCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvDcSts") {
    const InvDcSts *msg = (const InvDcSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvHvDcBusSts") {
    const InvHvDcBusSts *msg = (const InvHvDcBusSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvOpsSts") {
    const InvOpsSts *msg = (const InvOpsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvStatsSts") {
    const InvStatsSts *msg = (const InvStatsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvSts") {
    const InvSts *msg = (const InvSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvTempSts") {
    const InvTempSts *msg = (const InvTempSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvTempSts2") {
    const InvTempSts2 *msg = (const InvTempSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoAck") {
    const IsoAck *msg = (const IsoAck *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoAddrClaim") {
    const IsoAddrClaim *msg = (const IsoAddrClaim *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoCmdAddr") {
    const IsoCmdAddr *msg = (const IsoCmdAddr *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoReq") {
    const IsoReq *msg = (const IsoReq *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManAbort") {
    const IsoTpConnManAbort *msg = (const IsoTpConnManAbort *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManBam") {
    const IsoTpConnManBam *msg = (const IsoTpConnManBam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManCts") {
    const IsoTpConnManCts *msg = (const IsoTpConnManCts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManEom") {
    const IsoTpConnManEom *msg = (const IsoTpConnManEom *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManRts") {
    const IsoTpConnManRts *msg = (const IsoTpConnManRts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpXfer") {
    const IsoTpXfer *msg = (const IsoTpXfer *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAcStsRms") {
    const PmAcStsRms *msg = (const PmAcStsRms *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAcXferSwSts") {
    const PmAcXferSwSts *msg = (const PmAcXferSwSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAssocCfg") {
    const PmAssocCfg *msg = (const PmAssocCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAssocSts") {
    const PmAssocSts *msg = (const PmAssocSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmChgSts") {
    const PmChgSts *msg = (const PmChgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmDevMdCfg") {
    const PmDevMdCfg *msg = (const PmDevMdCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmDevMdSts") {
    const PmDevMdSts *msg = (const PmDevMdSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmFxccCfgCmd") {
    const PmFxccCfgCmd *msg = (const PmFxccCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmFxccCfgSts") {
    const PmFxccCfgSts *msg = (const PmFxccCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmGetConfigParam") {
    const PmGetConfigParam *msg = (const PmGetConfigParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmGetStatusParam") {
    const PmGetStatusParam *msg = (const PmGetStatusParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvCtrlCmd") {
    const PmInvCtrlCmd *msg = (const PmInvCtrlCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvLdShSts") {
    const PmInvLdShSts *msg = (const PmInvLdShSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvSts") {
    const PmInvSts *msg = (const PmInvSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmLithionicsCmd") {
    const PmLithionicsCmd *msg = (const PmLithionicsCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmLithionicsSts") {
    const PmLithionicsSts *msg = (const PmLithionicsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAssocCfg") {
    const PmParamAssocCfg *msg = (const PmParamAssocCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfg") {
    const PmParamAuxCfg *msg = (const PmParamAuxCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfgTrigDly") {
    const PmParamAuxCfgTrigDly *msg = (const PmParamAuxCfgTrigDly *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfgTrigLvl") {
    const PmParamAuxCfgTrigLvl *msg = (const PmParamAuxCfgTrigLvl *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxSts") {
    const PmParamAuxSts *msg = (const PmParamAuxSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamDevMdCfg") {
    const PmParamDevMdCfg *msg = (const PmParamDevMdCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnNakRsp") {
    const PmPpnNakRsp *msg = (const PmPpnNakRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnReadCmd") {
    const PmPpnReadCmd *msg = (const PmPpnReadCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnReadRsp") {
    const PmPpnReadRsp *msg = (const PmPpnReadRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnSessionCmd") {
    const PmPpnSessionCmd *msg = (const PmPpnSessionCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnSessionRsp") {
    const PmPpnSessionRsp *msg = (const PmPpnSessionRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnWriteCmd") {
    const PmPpnWriteCmd *msg = (const PmPpnWriteCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnWriteRsp") {
    const PmPpnWriteRsp *msg = (const PmPpnWriteRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmReq") {
    const PmReq *msg = (const PmReq *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmRpcCmd") {
    const PmRpcCmd *msg = (const PmRpcCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmSetConfigParam") {
    const PmSetConfigParam *msg = (const PmSetConfigParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmSwVerSts") {
    const PmSwVerSts *msg = (const PmSwVerSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ProdIdent") {
    const ProdIdent *msg = (const ProdIdent *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ProdInfoSts") {
    const ProdInfoSts *msg = (const ProdInfoSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccArraySts") {
    const SccArraySts *msg = (const SccArraySts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccBattSts") {
    const SccBattSts *msg = (const SccBattSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd") {
    const SccCfgCmd *msg = (const SccCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd2") {
    const SccCfgCmd2 *msg = (const SccCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd3") {
    const SccCfgCmd3 *msg = (const SccCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd4") {
    const SccCfgCmd4 *msg = (const SccCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd5") {
    const SccCfgCmd5 *msg = (const SccCfgCmd5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts") {
    const SccCfgSts *msg = (const SccCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts2") {
    const SccCfgSts2 *msg = (const SccCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts3") {
    const SccCfgSts3 *msg = (const SccCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts4") {
    const SccCfgSts4 *msg = (const SccCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts5") {
    const SccCfgSts5 *msg = (const SccCfgSts5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCmd") {
    const SccCmd *msg = (const SccCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzCfgCmd") {
    const SccEqlzCfgCmd *msg = (const SccEqlzCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzCfgSts") {
    const SccEqlzCfgSts *msg = (const SccEqlzCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzSts") {
    const SccEqlzSts *msg = (const SccEqlzSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts") {
    const SccSts *msg = (const SccSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts2") {
    const SccSts2 *msg = (const SccSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts3") {
    const SccSts3 *msg = (const SccSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts4") {
    const SccSts4 *msg = (const SccSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts5") {
    const SccSts5 *msg = (const SccSts5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts6") {
    const SccSts6 *msg = (const SccSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SetSysDateTime") {
    const SetSysDateTime *msg = (const SetSysDateTime *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SimpleCalibCfg") {
    const SimpleCalibCfg *msg = (const SimpleCalibCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SwVerSts") {
    const SwVerSts *msg = (const SwVerSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SysDateTimeSts") {
    const SysDateTimeSts *msg = (const SysDateTimeSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "VirtualTerminal") {
    const VirtualTerminal *msg = (const VirtualTerminal *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "XbResetCmd") {
    const XbResetCmd *msg = (const XbResetCmd *)ptzRecv->pvStruct;
    msg->Print();
}else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgCmd1") {
    const AcFltCtrlCfgCmd1 *msg = (const AcFltCtrlCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgCmd2") {
    const AcFltCtrlCfgCmd2 *msg = (const AcFltCtrlCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgSts1") {
    const AcFltCtrlCfgSts1 *msg = (const AcFltCtrlCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AcFltCtrlCfgSts2") {
    const AcFltCtrlCfgSts2 *msg = (const AcFltCtrlCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdAmbTemp") {
    const AgsCritCmdAmbTemp *msg = (const AgsCritCmdAmbTemp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcI") {
    const AgsCritCmdDcI *msg = (const AgsCritCmdDcI *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcSoc") {
    const AgsCritCmdDcSoc *msg = (const AgsCritCmdDcSoc *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdDcV") {
    const AgsCritCmdDcV *msg = (const AgsCritCmdDcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritCmdXferAcV") {
    const AgsCritCmdXferAcV *msg = (const AgsCritCmdXferAcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsAmbTemp") {
    const AgsCritStsAmbTemp *msg = (const AgsCritStsAmbTemp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcI") {
    const AgsCritStsDcI *msg = (const AgsCritStsDcI *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcSoc") {
    const AgsCritStsDcSoc *msg = (const AgsCritStsDcSoc *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsDcV") {
    const AgsCritStsDcV *msg = (const AgsCritStsDcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AgsCritStsXferAcV") {
    const AgsCritStsXferAcV *msg = (const AgsCritStsXferAcV *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AlarmCmd") {
    const AlarmCmd *msg = (const AlarmCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "AlarmSts") {
    const AlarmSts *msg = (const AlarmSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattCmd") {
    const BattCmd *msg = (const BattCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts1") {
    const BattSts1 *msg = (const BattSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts11") {
    const BattSts11 *msg = (const BattSts11 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts2") {
    const BattSts2 *msg = (const BattSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts3") {
    const BattSts3 *msg = (const BattSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts4") {
    const BattSts4 *msg = (const BattSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSts6") {
    const BattSts6 *msg = (const BattSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "BattSumm") {
    const BattSumm *msg = (const BattSumm *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgCmd1") {
    const ChgAcFltCfgCmd1 *msg = (const ChgAcFltCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgCmd2") {
    const ChgAcFltCfgCmd2 *msg = (const ChgAcFltCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgSts1") {
    const ChgAcFltCfgSts1 *msg = (const ChgAcFltCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcFltCfgSts2") {
    const ChgAcFltCfgSts2 *msg = (const ChgAcFltCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts1") {
    const ChgAcSts1 *msg = (const ChgAcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts2") {
    const ChgAcSts2 *msg = (const ChgAcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts3") {
    const ChgAcSts3 *msg = (const ChgAcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgAcSts4") {
    const ChgAcSts4 *msg = (const ChgAcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd1") {
    const ChgCfgCmd1 *msg = (const ChgCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd2") {
    const ChgCfgCmd2 *msg = (const ChgCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd3") {
    const ChgCfgCmd3 *msg = (const ChgCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgCmd4") {
    const ChgCfgCmd4 *msg = (const ChgCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts1") {
    const ChgCfgSts1 *msg = (const ChgCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts2") {
    const ChgCfgSts2 *msg = (const ChgCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts3") {
    const ChgCfgSts3 *msg = (const ChgCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCfgSts4") {
    const ChgCfgSts4 *msg = (const ChgCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgCmd") {
    const ChgCmd *msg = (const ChgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzCfgCmd") {
    const ChgEqlzCfgCmd *msg = (const ChgEqlzCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzCfgSts") {
    const ChgEqlzCfgSts *msg = (const ChgEqlzCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgEqlzSts") {
    const ChgEqlzSts *msg = (const ChgEqlzSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgStatsSts") {
    const ChgStatsSts *msg = (const ChgStatsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts") {
    const ChgSts *msg = (const ChgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts2") {
    const ChgSts2 *msg = (const ChgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ChgSts3") {
    const ChgSts3 *msg = (const ChgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts1") {
    const CommSts1 *msg = (const CommSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts2") {
    const CommSts2 *msg = (const CommSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "CommSts3") {
    const CommSts3 *msg = (const CommSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcCfgCmd3") {
    const DcSrcCfgCmd3 *msg = (const DcSrcCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcCmd") {
    const DcSrcCmd *msg = (const DcSrcCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcConnSts") {
    const DcSrcConnSts *msg = (const DcSrcConnSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts1") {
    const DcSrcSts1 *msg = (const DcSrcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts11") {
    const DcSrcSts11 *msg = (const DcSrcSts11 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts2") {
    const DcSrcSts2 *msg = (const DcSrcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts3") {
    const DcSrcSts3 *msg = (const DcSrcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts4") {
    const DcSrcSts4 *msg = (const DcSrcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DcSrcSts6") {
    const DcSrcSts6 *msg = (const DcSrcSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DeviceStateSync") {
    const DeviceStateSync *msg = (const DeviceStateSync *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "DiagMsg1") {
    const DiagMsg1 *msg = (const DiagMsg1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldBlk") {
    const FileDnldBlk *msg = (const FileDnldBlk *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldInf") {
    const FileDnldInf *msg = (const FileDnldInf *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "FileDnldSts") {
    const FileDnldSts *msg = (const FileDnldSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenCmd") {
    const GenCmd *msg = (const GenCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenDemandCmd") {
    const GenDemandCmd *msg = (const GenDemandCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenDemandSts") {
    const GenDemandSts *msg = (const GenDemandSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenRst") {
    const GenRst *msg = (const GenRst *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenStartCfgCmd") {
    const GenStartCfgCmd *msg = (const GenStartCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenStartCfgSts") {
    const GenStartCfgSts *msg = (const GenStartCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenSts1") {
    const GenSts1 *msg = (const GenSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "GenSts2") {
    const GenSts2 *msg = (const GenSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InstAssign") {
    const InstAssign *msg = (const InstAssign *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InstSts") {
    const InstSts *msg = (const InstSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcFltCfgCmd1") {
    const InvAcFltCfgCmd1 *msg = (const InvAcFltCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcFltCfgSts1") {
    const InvAcFltCfgSts1 *msg = (const InvAcFltCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts1") {
    const InvAcSts1 *msg = (const InvAcSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts2") {
    const InvAcSts2 *msg = (const InvAcSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts3") {
    const InvAcSts3 *msg = (const InvAcSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvAcSts4") {
    const InvAcSts4 *msg = (const InvAcSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvApsSts") {
    const InvApsSts *msg = (const InvApsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd1") {
    const InvCfgCmd1 *msg = (const InvCfgCmd1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd2") {
    const InvCfgCmd2 *msg = (const InvCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd3") {
    const InvCfgCmd3 *msg = (const InvCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgCmd4") {
    const InvCfgCmd4 *msg = (const InvCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts1") {
    const InvCfgSts1 *msg = (const InvCfgSts1 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts2") {
    const InvCfgSts2 *msg = (const InvCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts3") {
    const InvCfgSts3 *msg = (const InvCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCfgSts4") {
    const InvCfgSts4 *msg = (const InvCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvCmd") {
    const InvCmd *msg = (const InvCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvDcSts") {
    const InvDcSts *msg = (const InvDcSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvHvDcBusSts") {
    const InvHvDcBusSts *msg = (const InvHvDcBusSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvOpsSts") {
    const InvOpsSts *msg = (const InvOpsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvStatsSts") {
    const InvStatsSts *msg = (const InvStatsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvSts") {
    const InvSts *msg = (const InvSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvTempSts") {
    const InvTempSts *msg = (const InvTempSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "InvTempSts2") {
    const InvTempSts2 *msg = (const InvTempSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoAck") {
    const IsoAck *msg = (const IsoAck *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoAddrClaim") {
    const IsoAddrClaim *msg = (const IsoAddrClaim *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoCmdAddr") {
    const IsoCmdAddr *msg = (const IsoCmdAddr *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoReq") {
    const IsoReq *msg = (const IsoReq *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManAbort") {
    const IsoTpConnManAbort *msg = (const IsoTpConnManAbort *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManBam") {
    const IsoTpConnManBam *msg = (const IsoTpConnManBam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManCts") {
    const IsoTpConnManCts *msg = (const IsoTpConnManCts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManEom") {
    const IsoTpConnManEom *msg = (const IsoTpConnManEom *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpConnManRts") {
    const IsoTpConnManRts *msg = (const IsoTpConnManRts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "IsoTpXfer") {
    const IsoTpXfer *msg = (const IsoTpXfer *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAcStsRms") {
    const PmAcStsRms *msg = (const PmAcStsRms *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAcXferSwSts") {
    const PmAcXferSwSts *msg = (const PmAcXferSwSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAssocCfg") {
    const PmAssocCfg *msg = (const PmAssocCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmAssocSts") {
    const PmAssocSts *msg = (const PmAssocSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmChgSts") {
    const PmChgSts *msg = (const PmChgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmDevMdCfg") {
    const PmDevMdCfg *msg = (const PmDevMdCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmDevMdSts") {
    const PmDevMdSts *msg = (const PmDevMdSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmFxccCfgCmd") {
    const PmFxccCfgCmd *msg = (const PmFxccCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmFxccCfgSts") {
    const PmFxccCfgSts *msg = (const PmFxccCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmGetConfigParam") {
    const PmGetConfigParam *msg = (const PmGetConfigParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmGetStatusParam") {
    const PmGetStatusParam *msg = (const PmGetStatusParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvCtrlCmd") {
    const PmInvCtrlCmd *msg = (const PmInvCtrlCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvLdShSts") {
    const PmInvLdShSts *msg = (const PmInvLdShSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmInvSts") {
    const PmInvSts *msg = (const PmInvSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmLithionicsCmd") {
    const PmLithionicsCmd *msg = (const PmLithionicsCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmLithionicsSts") {
    const PmLithionicsSts *msg = (const PmLithionicsSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAssocCfg") {
    const PmParamAssocCfg *msg = (const PmParamAssocCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfg") {
    const PmParamAuxCfg *msg = (const PmParamAuxCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfgTrigDly") {
    const PmParamAuxCfgTrigDly *msg = (const PmParamAuxCfgTrigDly *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxCfgTrigLvl") {
    const PmParamAuxCfgTrigLvl *msg = (const PmParamAuxCfgTrigLvl *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamAuxSts") {
    const PmParamAuxSts *msg = (const PmParamAuxSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmParamDevMdCfg") {
    const PmParamDevMdCfg *msg = (const PmParamDevMdCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnNakRsp") {
    const PmPpnNakRsp *msg = (const PmPpnNakRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnReadCmd") {
    const PmPpnReadCmd *msg = (const PmPpnReadCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnReadRsp") {
    const PmPpnReadRsp *msg = (const PmPpnReadRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnSessionCmd") {
    const PmPpnSessionCmd *msg = (const PmPpnSessionCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnSessionRsp") {
    const PmPpnSessionRsp *msg = (const PmPpnSessionRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnWriteCmd") {
    const PmPpnWriteCmd *msg = (const PmPpnWriteCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmPpnWriteRsp") {
    const PmPpnWriteRsp *msg = (const PmPpnWriteRsp *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmReq") {
    const PmReq *msg = (const PmReq *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmRpcCmd") {
    const PmRpcCmd *msg = (const PmRpcCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmSetConfigParam") {
    const PmSetConfigParam *msg = (const PmSetConfigParam *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "PmSwVerSts") {
    const PmSwVerSts *msg = (const PmSwVerSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ProdIdent") {
    const ProdIdent *msg = (const ProdIdent *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "ProdInfoSts") {
    const ProdInfoSts *msg = (const ProdInfoSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccArraySts") {
    const SccArraySts *msg = (const SccArraySts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccBattSts") {
    const SccBattSts *msg = (const SccBattSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd") {
    const SccCfgCmd *msg = (const SccCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd2") {
    const SccCfgCmd2 *msg = (const SccCfgCmd2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd3") {
    const SccCfgCmd3 *msg = (const SccCfgCmd3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd4") {
    const SccCfgCmd4 *msg = (const SccCfgCmd4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgCmd5") {
    const SccCfgCmd5 *msg = (const SccCfgCmd5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts") {
    const SccCfgSts *msg = (const SccCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts2") {
    const SccCfgSts2 *msg = (const SccCfgSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts3") {
    const SccCfgSts3 *msg = (const SccCfgSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts4") {
    const SccCfgSts4 *msg = (const SccCfgSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCfgSts5") {
    const SccCfgSts5 *msg = (const SccCfgSts5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccCmd") {
    const SccCmd *msg = (const SccCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzCfgCmd") {
    const SccEqlzCfgCmd *msg = (const SccEqlzCfgCmd *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzCfgSts") {
    const SccEqlzCfgSts *msg = (const SccEqlzCfgSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccEqlzSts") {
    const SccEqlzSts *msg = (const SccEqlzSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts") {
    const SccSts *msg = (const SccSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts2") {
    const SccSts2 *msg = (const SccSts2 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts3") {
    const SccSts3 *msg = (const SccSts3 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts4") {
    const SccSts4 *msg = (const SccSts4 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts5") {
    const SccSts5 *msg = (const SccSts5 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SccSts6") {
    const SccSts6 *msg = (const SccSts6 *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SetSysDateTime") {
    const SetSysDateTime *msg = (const SetSysDateTime *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SimpleCalibCfg") {
    const SimpleCalibCfg *msg = (const SimpleCalibCfg *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SwVerSts") {
    const SwVerSts *msg = (const SwVerSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "SysDateTimeSts") {
    const SysDateTimeSts *msg = (const SysDateTimeSts *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "VirtualTerminal") {
    const VirtualTerminal *msg = (const VirtualTerminal *)ptzRecv->pvStruct;
    msg->Print();
}
else if(PGNTABLE[ptzRecv->ulPgn] == "XbResetCmd") {
    const XbResetCmd *msg = (const XbResetCmd *)ptzRecv->pvStruct;
    msg->Print();
}