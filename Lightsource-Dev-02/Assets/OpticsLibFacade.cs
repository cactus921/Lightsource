using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;

static class Constants
{
    public const int max_beam_points = 20;
    public const double Pi = 3.14159;
    public const double Epsilon = 0.0001;
}

public class OpticsLibFacade
{
    //private double[] beam_points_arr;

    /********************************  DLL Imports  *****************************************/

    [DllImport("OpticsLibrary")]
    private static extern IntPtr ExtCreateNativeScene();

    [DllImport("OpticsLibrary")]
    private static extern IntPtr ExtCreateNativeBeam(IntPtr scene, double[] position, double[] direction);

    [DllImport("OpticsLibrary")]
    private static extern IntPtr ExtCreateNativeSceneElement(IntPtr scene, double[] position, double[] normal);

    [DllImport("OpticsLibrary")]
    private static extern void ExtSetElementTransform(IntPtr scene, IntPtr element, double[] position, double[] normal);

    [DllImport("OpticsLibrary")]
    private static extern void ExtGetBeamPoints(IntPtr scene, IntPtr beam, double[] beam_points_arr);

    //[DllImport("OpticsLibrary.dll", CallingConvention = CallingConvention.Cdecl)]
    //private static extern void GetBeamPoints(IntPtr scene, IntPtr beam, [MarshalAs(UnmanagedType.SafeArray, SafeArraySubType = VarEnum.VT_R8)] ref double[] arr);


    /********************************  Class Methods *****************************************/

    public OpticsLibFacade()
    {

    }

    private void OnInvalidPtr()
    {
        Debug.LogError("Error calling native method: Invalid pointer provided. Call to native library stopped.");
    }


    /********************************  Method Wrappers  *****************************************/

    public IntPtr CreateNativeScene()
    {
        IntPtr p = ExtCreateNativeScene();
        if (p == IntPtr.Zero)
        {
            Debug.LogError("Error creating native scene!");
        }
        return p;
    }

    public IntPtr CreateNativeBeam(IntPtr scene_object, Vector3 position, Vector3 normal)
    {
        double[] pos_arr = new double[3] { (double)position.x, (double)position.y, (double)position.z };
        Vector3 normalized = Vector3.Normalize(normal);
        double[] normal_arr = new double[3] { (double)normalized.x, (double)normalized.y, (double)normalized.z };
        IntPtr p = ExtCreateNativeBeam(scene_object, pos_arr, normal_arr);
        if (p == IntPtr.Zero)
        {
            Debug.LogError("Error creating native beam!");
        }
        return p;
    }

    public IntPtr CreateNativeSceneElement(IntPtr scene_object, Vector3 position, Vector3 direction)
    {
        double[] pos_arr = new double[3] { (double)position.x, (double)position.y, (double)position.z };
        Vector3 normalized = Vector3.Normalize(direction);
        double[] normal_arr = new double[3] { (double)normalized.x, (double)normalized.y, (double)normalized.z };
        IntPtr p = ExtCreateNativeSceneElement(scene_object, pos_arr, normal_arr);
        if (p == IntPtr.Zero)
        {
            Debug.LogError("Error creating native scene element!");
        }
        return p;
    }

    public void SetElementTransform(IntPtr scene_object, IntPtr element_object, Vector3 position, Vector3 direction)
    {
        if (scene_object == IntPtr.Zero || element_object == IntPtr.Zero)
        {
            OnInvalidPtr();
        }
        else
        {
            double[] pos_arr = new double[3] { (double)position.x, (double)position.y, (double)position.z };
            Vector3 normalized = Vector3.Normalize(direction);
            double[] normal_arr = new double[3] { (double)normalized.x, (double)normalized.y, (double)normalized.z };
            ExtSetElementTransform(scene_object, element_object, pos_arr, normal_arr);
        }
    }

    /* All changes are done on the points list. */
    public void GetBeamPoints(IntPtr scene_object, IntPtr beam_object, List<Vector3> points_list)
    {
        double[] beam_points_arr = new double[Constants.max_beam_points * 3];
        ExtGetBeamPoints(scene_object, beam_object, beam_points_arr);

        Vector3 prevPoint = new Vector3();
        for (int i = 0; i < Constants.max_beam_points; i++)
        {
            Vector3 point = new Vector3((float)beam_points_arr[i * 3],
                                        (float)beam_points_arr[i * 3 + 1],
                                        (float)beam_points_arr[i * 3 + 2]);
            //Debug.Log(point);
            if (Vector3.Distance(point, prevPoint) > 0.0f || i == 0)
            {
                if (i >= points_list.Count)
                {
                    points_list.Add(point);
                }
                else
                {
                    points_list[i] = point;
                }
            }
            prevPoint = point;
        }

    }


}
