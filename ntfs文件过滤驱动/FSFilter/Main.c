#include "FsFilter.h"
#include "Read.h"
#include "Write.h"
//////////////////////////////////////////////////////////////////////////
// Function prototypes

VOID FsFilterUnload(
    __in PDRIVER_OBJECT DriverObject
    );

//////////////////////////////////////////////////////////////////////////
// Global data

PDRIVER_OBJECT   g_fsFilterDriverObject = NULL;

FAST_IO_DISPATCH g_fastIoDispatch =
{
    sizeof(FAST_IO_DISPATCH),
    FsFilterFastIoCheckIfPossible,
    FsFilterFastIoRead,
    FsFilterFastIoWrite,
    FsFilterFastIoQueryBasicInfo,
    FsFilterFastIoQueryStandardInfo,
    FsFilterFastIoLock,
    FsFilterFastIoUnlockSingle,
    FsFilterFastIoUnlockAll,
    FsFilterFastIoUnlockAllByKey,
    FsFilterFastIoDeviceControl,
    NULL,
    NULL,
    FsFilterFastIoDetachDevice,
    FsFilterFastIoQueryNetworkOpenInfo,
    NULL,
    FsFilterFastIoMdlRead,
    FsFilterFastIoMdlReadComplete,
    FsFilterFastIoPrepareMdlWrite,
    FsFilterFastIoMdlWriteComplete,
    FsFilterFastIoReadCompressed,
    FsFilterFastIoWriteCompressed,
    FsFilterFastIoMdlReadCompleteCompressed,
    FsFilterFastIoMdlWriteCompleteCompressed,
    FsFilterFastIoQueryOpen,
    NULL,
    NULL,
    NULL,
};

//////////////////////////////////////////////////////////////////////////
// DriverEntry - Entry point of the driver
NTSTATUS DriverEntry(
    __inout PDRIVER_OBJECT  DriverObject,
    __in    PUNICODE_STRING RegistryPath
    )
{    
	UNREFERENCED_PARAMETER(RegistryPath);
	DbgBreakPoint();
    NTSTATUS status = STATUS_SUCCESS;
    ULONG    i      = 0;

    //ASSERT(FALSE); // This will break to debugger

    //
    // Store our driver object.
    //

    g_fsFilterDriverObject = DriverObject;
    
    //
    //  Initialize the driver object dispatch table.
    //

    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; ++i) 
    {
        DriverObject->MajorFunction[i] = FsFilterDispatchPassThrough;
    }

	//��һ���ļ�ʱ������CreateFile��irpstack�е�FileObject��ΪCreateFile���صľ��
	//irpsp->paramenters->create�д����CreateFile����ز���,������������������ļ�����
	//ע�⣺ObQueryNameStringֻ����CREATE IRP��CLEAN UP IRP�Ĵ�����ʹ��,
	//���������Լ��·�IRP,һ�㶼��ʹϵͳ��������.
    DriverObject->MajorFunction[IRP_MJ_CREATE] = FsFilterDispatchCreate; 

	//��ȡһ���ļ�����ReadFile����ʱ��irpstack->FileObjectΪ��IRP_MJ_CREATE�з��صľ��
	//���ж�ȡ�Ľ��������irp�е�mdl��systembuffer���ҵ�
	//��ȡ��ƫ�ƺ��ֽڿ�����irpstack->paramenter�е�read���ҵ�
	DriverObject->MajorFunction[IRP_MJ_READ] = FsFilterDispatchRead;


	//дһ���ļ�����WriteFile����ʱ��irpstack->FileObjectΪ��IRP_MJ_CREATE�з��صľ��
	//����д�����ݿ�����irp�е�mdl��systembuffer���ҵ�
	//д��ƫ�ƺ��ֽڿ�����irpstack->paramenter�е�write���ҵ�
	DriverObject->MajorFunction[IRP_MJ_WRITE] = FsFilterDispatchWrite;


    //
    // Set fast-io dispatch table.
    //

    DriverObject->FastIoDispatch = &g_fastIoDispatch;

    //
    //  Registered callback routine for file system changes.
    //

    status = IoRegisterFsRegistrationChange(DriverObject, FsFilterNotificationCallback); 
    if (!NT_SUCCESS(status)) 
    {
        return status;
    }

    //
    // Set driver unload routine (debug purpose only).
    //

    DriverObject->DriverUnload = FsFilterUnload;

    return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// Unload routine

VOID FsFilterUnload(
    __in PDRIVER_OBJECT DriverObject
    )
{
    ULONG           numDevices = 0;
    ULONG           i          = 0;    
    LARGE_INTEGER   interval;
    PDEVICE_OBJECT  devList[DEVOBJ_LIST_SIZE];

    interval.QuadPart = (5 * DELAY_ONE_SECOND); //delay 5 seconds

    //
    //  Unregistered callback routine for file system changes.
    //

    IoUnregisterFsRegistrationChange(DriverObject, FsFilterNotificationCallback);

    //
    //  This is the loop that will go through all of the devices we are attached
    //  to and detach from them.
    //

    for (;;)
    {
        IoEnumerateDeviceObjectList(
            DriverObject,
            devList,
            sizeof(devList),
            &numDevices);

        if (0 == numDevices)
        {
            break;
        }

        numDevices = min(numDevices, RTL_NUMBER_OF(devList));

        for (i = 0; i < numDevices; ++i) 
        {
            FsFilterDetachFromDevice(devList[i]);
            ObDereferenceObject(devList[i]);
        }
        
        KeDelayExecutionThread(KernelMode, FALSE, &interval);
    }
}

//////////////////////////////////////////////////////////////////////////
// Misc

BOOLEAN FsFilterIsMyDeviceObject(
    __in PDEVICE_OBJECT DeviceObject
    )
{
    return DeviceObject->DriverObject == g_fsFilterDriverObject;
}
