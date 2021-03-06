using System;
using System.Runtime.InteropServices;

namespace mywowNet
{
    public partial class ResourceLoader
    {
        const string DllName = @"mywow_dll.dll";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "ResourceLoader_loadWDT")]
        public static extern IntPtr ResourceLoader_loadWDT([MarshalAs(UnmanagedType.LPStr, SizeConst = 64)]string mapname, int id, [MarshalAsAttribute(UnmanagedType.I1)] bool simple);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "ResourceLoader_unloadWDT")]
        public static extern void ResourceLoader_unloadWDT(IntPtr wdt);
    }
}
