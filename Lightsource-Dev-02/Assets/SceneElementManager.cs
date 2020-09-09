using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class SceneElementManager : MonoBehaviour
{
    private IntPtr m_SceneNativeObj, m_NativeObj;
    private OpticsLibFacade m_Lib;
    private SceneManager m_Scene;
    private Vector3 m_CurrentPosition, m_CurrentDirection;


    //private Quaternion m_CurrentRotation;

    public void Init(SceneManager scene, IntPtr scene_native_object, Vector3 position, Vector3 direction)
    {
        m_Scene = scene;
        m_SceneNativeObj = scene_native_object;
        m_NativeObj = m_Lib.CreateNativeSceneElement(scene_native_object, position, Vector3.Normalize(direction));

        transform.position = position;
        transform.forward = direction;
        m_CurrentPosition = transform.position;
        m_CurrentDirection = transform.forward;
    }

    private void Awake()
    {
        m_Lib = new OpticsLibFacade();
        m_CurrentPosition = new Vector3();
        m_CurrentDirection = new Vector3();
        //m_CurrentRotation = new Quaternion();
    }

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (transform.position != m_CurrentPosition || transform.forward != m_CurrentDirection)
        {
            m_CurrentPosition = transform.position;
            m_CurrentDirection = transform.forward;
            m_Lib.SetElementTransform(m_SceneNativeObj, m_NativeObj, m_CurrentPosition, Vector3.Normalize(m_CurrentDirection));
            m_Scene.OnSceneChanged();
        }
    }
}
