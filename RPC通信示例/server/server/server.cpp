// server.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"rpc_h.h"
#include"rpc_s.c"

#pragma comment(lib, "Rpcrt4.lib")

int main()
{
    RPC_STATUS Status = RPC_S_OK;

    Status = RpcServerUseProtseqEp(
        RPC_WSTR(L"ncalrpc"),                      //使用LPC通信
        RPC_C_PROTSEQ_MAX_REQS_DEFAULT,
        RPC_WSTR(L"{0AD3C2E4-C14D-48E1-9CAF-502CFD189EE0}"),
        NULL);
    if (Status != RPC_S_OK)
        return 0;

    Status = RpcServerRegisterIfEx(
        RpcService_v1_0_s_ifspec, // Interface to register.
        NULL,
        NULL, // Use the MIDL generated entry-point vector.
        RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH,
        0,
        NULL);

    if (Status != RPC_S_OK)
    {
        return 0;
    }

    Status = RpcServerListen(1, 20, FALSE);

    return 0;
}

HRESULT RpcCall(PParam lpParam)
{
    printf("RPC Call\n");
    return 1;
}

/* [async] */ void  RpcAsyncCall(
    /* [in] */ PRPC_ASYNC_STATE RpcAsyncCall_AsyncHandle,
    /* [out][in] */ PParam lpParam)
{
    printf("RPC Asysc Call\n");
    Sleep(4000);
    BOOL bRet = 1;

    lpParam->Type = 1;
    lpParam->uParam.Param.a = 2;
    RpcAsyncCompleteCall(RpcAsyncCall_AsyncHandle, &bRet);
    return;
}

void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len)
{
    return(malloc(len));
}

void __RPC_USER  midl_user_free(void __RPC_FAR *ptr)
{
    free(ptr);
}
