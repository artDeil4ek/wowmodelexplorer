using System;
using System.Runtime.InteropServices;

namespace mywowNet
{
    public partial class ManualMeshServices
    {
        const string DllName = @"mywow_dll.dll";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "ManualMeshServices_addGridMesh")]
        [return: MarshalAsAttribute(UnmanagedType.I1)]
        public static extern bool ManualMeshServices_addGridMesh(
            [MarshalAs(UnmanagedType.LPStr, SizeConst = 64)]string name,
            uint xzCount,
            float gridSize,
            SColor color);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "ManualMeshServices_removeMesh")]
        public static extern void ManualMeshServices_removeMesh(
            [MarshalAs(UnmanagedType.LPStr, SizeConst = 64)]string name);
    }
}