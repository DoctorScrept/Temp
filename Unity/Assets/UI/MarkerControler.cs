using UnityEngine;
using UnityEngine.UI;

public class MarkerControler : MonoBehaviour
{
	public GameObject marker;
	public Text u1Text;
	public Text u2Text;
	public Text iText;
	public Plotter plotter;
	
	void Awake() {
		gameObject.SetActive(false);
	}

	void Start () {
		marker.transform.position = plotter.transform.position;
	}

	void Update ()
	{
		if(Input.GetMouseButton(0))
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
			marker.transform.position = hit.point;
			SetText(hit.point, plotter.GetSurfaceBySlot(hit.collider.gameObject));
		}
	}

	private void SetText(Vector3 point, Surface surface)
	{
		u1Text.text = point.x.ToString();
		u2Text.text = point.y.ToString();
		iText.text = point.z.ToString();
	}

	public bool IsEnable {
		get { return gameObject.activeSelf; }
		set { gameObject.SetActive(value); }
	}
}
