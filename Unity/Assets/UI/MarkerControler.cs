using UnityEngine;
using UnityEngine.UI;

public class MarkerControler : MonoBehaviour
{
	private const string VALUE_FORMAT = "F1";

	public GameObject marker;
	public Text u1Text;
	public Text u2Text;
	public Text iText;
	public Plotter plotter;
	
	void Awake() {
		gameObject.SetActive(false);
		point = Vector3.zero;
	}

	void Start () {
		IsActive = true;
		marker.transform.position = plotter.transform.position;
		SetText(Vector3.zero, null);
	}

	void Update ()
	{
		if(Input.GetMouseButton(0) && IsActive)
		{
			MarkByScreenPoint(Input.mousePosition);
		}
	}

	public void MarkByScreenPoint(Vector3 screenPoint)
	{
		Ray ray = Camera.main.ScreenPointToRay(screenPoint);
		RaycastHit hit;
		if (Physics.Raycast(ray, out hit))
		{
			point = hit.point;
			marker.transform.position = hit.point;
			SetText(hit.point, plotter.GetSurfaceBySlot(hit.collider.gameObject));
		}
	}

	private void SetText(Vector3 point, Surface surface)
	{
		u1Text.text = "Ib = " + (point.x * 50).ToString(VALUE_FORMAT) + " мкА";
		u2Text.text = "Ik = " + (point.y * 10).ToString(VALUE_FORMAT) + " мА";
		iText.text = "Uke = " + (point.z).ToString(VALUE_FORMAT) + " В";
	}

	public bool IsEnable {
		get { return gameObject.activeSelf; }
		set { gameObject.SetActive(value); }
	}

	public bool IsActive { get; set; }

	Vector3 point;
	public Vector3 GetPoint() {
		return point;
	}
}
