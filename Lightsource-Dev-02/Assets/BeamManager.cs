using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class BeamManager : MonoBehaviour
{

    public GameObject beamPointPrefab;
    public List<Vector3> beamPointsLocations;
    public List<GameObject> beamPoints;

    private IntPtr m_SceneNativeObj, m_NativeObj;
    private LineRenderer m_Line;
    private OpticsLibFacade m_Lib;
    private SceneManager m_Scene;

    public void Init(SceneManager scene, IntPtr scene_native_object, Vector3 start_position, Vector3 start_direction)
    {
        m_Scene = scene;
        m_SceneNativeObj = scene_native_object;
        m_NativeObj = m_Lib.CreateNativeBeam(scene_native_object, start_position, Vector3.Normalize(start_direction));

        UpdateBeamPoints();
    }

    private void Awake()
    {
        beamPointsLocations = new List<Vector3>();
        beamPoints = new List<GameObject>();
        m_Lib = new OpticsLibFacade();

        m_Line = gameObject.GetComponent<LineRenderer>();
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    

    // Update is called once per frame
    void Update()
    {
        
    }


    public void UpdateBeamPoints()
    {
        //Debug.Log(beamPointsLocations.Count);
        m_Lib.GetBeamPoints(m_SceneNativeObj, m_NativeObj, beamPointsLocations);
        //Debug.Log(beamPointsLocations.Count);
        for (int i = 0; i < beamPointsLocations.Count; i++)
        {
            // New beam point
            if(i >= beamPoints.Count)
            {
                GameObject newPoint = Instantiate(beamPointPrefab, beamPointsLocations[i], Quaternion.identity, transform);
                beamPoints.Add(newPoint);
            }

            // Existing beam point
            else if( Vector3.Distance(beamPointsLocations[i], beamPoints[i].transform.position) > Constants.Epsilon )
            {
                beamPoints[i].transform.position = beamPointsLocations[i];
            }
        }
        
        // Delete extra beam points
        if(beamPoints.Count > beamPointsLocations.Count)
        {
            for(int j = beamPointsLocations.Count; j < beamPoints.Count; j++)
            {
                GameObject.Destroy(beamPoints[j]);
            }
            beamPoints.RemoveRange(beamPointsLocations.Count, beamPoints.Count - beamPointsLocations.Count);
        }

        m_Line.positionCount = beamPointsLocations.Count;
        m_Line.SetPositions(beamPointsLocations.ToArray());
    }
}
