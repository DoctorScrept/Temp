using UnityEngine;
using System.Collections;

public class TPS_RPG_CamCon : MonoBehaviour {
	
	
	//public Transform Cam_main;
	public Transform target;
	public float min_distance = 10.0f;
	public float max_distance = 50.0f;

	//double
	public float distance = 50.0f; //How far away to orbit
	float xSpeed = 125.0f; //X sensitivity
	float ySpeed = 50.0f; //Y sensitivity

	private float x = 0.0f; //Angle of the y rotation?
	private float y = 0.0f; //Angle of the x rotation?
	
	Quaternion rotation = Quaternion.Euler(0, 0, 0);
	
	void Start ()
	{
		Vector3 angles = transform.eulerAngles;
	    x = angles.y;
	    y = angles.x;
	}
	
	// Update is called once per frame
	void Update () 
	{
		if (target) //если есть обьект на который завязана камера
		{
			if(Input.GetMouseButton(1))//менять угол обзора только при зажатой клавише
			{//There's a target
	 	       //Change the angles by the mouse movement
		        x += Input.GetAxis("Mouse X") * xSpeed * 0.02f;
	   	 	    y -= Input.GetAxis("Mouse Y") * ySpeed * 0.02f;
			
	        	//Rotate the camera to those angles 
				rotation = Quaternion.Euler(y, x, 0);
	        	transform.rotation = rotation;
			}
		
            //при прокрутке колеса миши менять растояние до обьекта
			if(Input.GetAxis("Mouse ScrollWheel")!=0)
			{
                distance=distance-Input.GetAxis("Mouse ScrollWheel")*5;
                if (distance < min_distance) {distance=min_distance;}
                if (distance > max_distance) {distance=max_distance;}
            }
			
			//применяет все изменения
			transform.position = rotation * (new Vector3(0.0f, 0.0f, -distance)) + target.position;  
		}
	}
}
