using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.Runtime.InteropServices;
using System;



public class SceneManager : MonoBehaviour
{
    public GameObject beamPrefab, sceneElementPrefab;
    
    private IntPtr m_NativeScene, m_NativeBeam, m_NativeSceneElement;
    private OpticsLibFacade m_Lib;
    private BeamManager m_Beam;
    //private List<Vector3> beamPoints;

    private void Awake()
    {
        m_Lib = new OpticsLibFacade();
    }

    // Start is called before the first frame update
    void Start()
    {
        m_NativeScene = m_Lib.CreateNativeScene();

        GameObject beam = Instantiate(beamPrefab, new Vector3(), Quaternion.identity, transform);
        m_Beam = beam.GetComponent<BeamManager>();
        m_Beam.Init(this, m_NativeScene, new Vector3(), new Vector3(0.0f, 0.0f, 1.0f));


        /*

        m_NativeScene = ExtCreateNativeScene();

        double[] b_pos = new double[3] { 0.0, 0.0, 0.0 };
        Vector3 b_dirvec = Vector3.Normalize(new Vector3(0.0f, 0.0f, 1.0f));
        double[] b_dir = new double[3] { (double)b_dirvec.x, (double)b_dirvec.y, (double)b_dirvec.z };
        m_NativeBeam = ExtCreateNativeBeam(m_NativeScene, b_pos, b_dir);

        double[] el_pos = new double[3] { 0.0, 0.0, 20.0 };
        Vector3 el_dirvec = Vector3.Normalize(new Vector3(-1.0f, 0.0f, -1.0f));
        double[] el_dir = new double[3] { (double)el_dirvec.x, (double)el_dirvec.y, (double)el_dirvec.z };
        m_NativeSceneElement = ExtCreateNativeSceneElement(m_NativeScene, el_pos, el_dir);

        double[] el2_pos = new double[3] { -20.0, 0.0, 20.0 };
        Vector3 el2_dirvec = Vector3.Normalize(new Vector3(1.0f, 0.0f, -1.0f));
        double[] el2_dir = new double[3] { (double)el2_dirvec.x, (double)el2_dirvec.y, (double)el2_dirvec.z };
        //ExtCreateNativeSceneElement(m_NativeScene, el2_pos, el2_dir);

        double[] el3_pos = new double[3] { -20.0, 0.0, 0.0 };
        Vector3 el3_dirvec = Vector3.Normalize(new Vector3(1.0f, 0.0f, 1.0f));
        double[] el3_dir = new double[3] { (double)el3_dirvec.x, (double)el3_dirvec.y, (double)el3_dirvec.z };
        //ExtCreateNativeSceneElement(m_NativeScene, el3_pos, el3_dir);

        beamPoints = new List<Vector3>();       
        GetBeamPoints(beam_points_arr, beamPoints);
        foreach (Vector3 p in beamPoints)
            Instantiate(beamPointPrefab, p, Quaternion.identity, transform);
            */


    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.Space))
        {
            Vector3 pos = new Vector3(0.0f, 0.0f, 20.0f);
            Vector3 dir = Vector3.Normalize(new Vector3(-1.0f, 0.0f, -1.0f));
            GameObject el1 = AddElement(pos, dir);
        }
        

    }

    private void OnDrawGizmos()
    {

    }


    private GameObject AddElement(Vector3 position, Vector3 direction)
    {
        GameObject instance = Instantiate(sceneElementPrefab, position, Quaternion.identity, transform);
        instance.transform.LookAt(instance.transform.position + direction);
        instance.GetComponent<SceneElementManager>().Init(this, m_NativeScene, position, Vector3.Normalize(direction));
        
        if(m_Beam.isActiveAndEnabled)
        {
            m_Beam.UpdateBeamPoints();
        }
        return instance;
    }

    public void OnSceneChanged()
    {
        m_Beam.UpdateBeamPoints();
    }


    

}
