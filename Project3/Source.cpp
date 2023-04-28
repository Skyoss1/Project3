#include<iostream>
using namespace std;
#include<string>
#include<iomanip>
#include<fstream>
#pragma warning(suppress : 4996)

// khởi tạo các struct
struct Date
{
	int d, m, y;
	Date() : d(0), m(0), y(0) {};
};

struct HangHoa
{
	string ID, Name;
	bool Status;
	int Soluong;
	float DonGia;
	double ThanhTien;
	Date HanSuDung;
	HangHoa() : ID(""), Name(""), Status(0), Soluong(0), DonGia(0), ThanhTien(0) {};
};

struct Node
{
	HangHoa data;
	Node* pNext;
};

struct LIST
{
	Node* pHead;
	Node* pTail;
};
void swap(HangHoa& x, HangHoa& y)//Hoán đổi thứ tự hàng hóa khi sắp xếp khi sử dụng sort or delete
{
	HangHoa t = x;
	x = y;
	y = t;
}
void sortListByID(LIST& l) {
	for (Node* P = l.pHead; P->pNext != NULL; P = P->pNext)
		for (Node* Q = P->pNext; Q != NULL; Q = Q->pNext)
			if (P->data.ID > Q->data.ID)
				swap(P->data, Q->data);
}
//Tạo node chứa dữ liệu h
Node* createNode(HangHoa h) {
	Node* Hanghoa = new Node;//cấp phát vùng nhớ cho Hanghoa
	if (Hanghoa == NULL)
		return NULL;
	Hanghoa->data = h; // Ghi dữ liệu vào Data
	Hanghoa->pNext = NULL;// cho pNext chỏ tới NULL
	return Hanghoa; // trả về Node mới đã có giá trị
}
void create_list(LIST& l)// khởi tạo danh sách rỗng
{
	l.pHead = NULL;
	l.pTail = NULL;
}
void add_last_list(LIST& l, Node* p)// thêm cuối danh sách
{
	if (l.pHead == NULL)
		l.pHead = l.pTail = p;
	else
	{
		Node* tmp = l.pHead;
		while (tmp->pNext != NULL)
			tmp = tmp->pNext;
		tmp->pNext = p;
	}
}
int len_list(LIST l) // tính độ dài danh sách
{
	int i = 0;//biến đếm
	Node* k = l.pHead;
	while (k != NULL)//duyệt đến cuối ds
	{
		i++;//đếm số Node
		k = k->pNext;//cho k trỏ đến node tiếp theo
	}
	return i; //trả về số node của l
}

void* Mystrlwr(char*& s) {
	for (char* p = s; *p; ++p) *p = std::tolower(*p);
}

void standard_name(string& name)
{
	while (name[0] == ' ')
		name.erase(0, 1);
	while (name[name.length() - 1] == ' ')
		name.pop_back();
	int i = 0;
	while (i < name.length() - 1)
	{
		if (name[i] == ' ' && name[i + 1] == ' ')
		{
			name.erase(i, 1);
			i--;
		}
		i++;
	}
	int l = name.length();
	char* c_name = name.c_str();
	Mystrlwr(c_name);
	name[0] = name[0] - 32;
	for (int i = 0; i < l - 1; i++)
		if (name[i] == ' ' && name[i + 1] != ' ')
			name[i + 1] = name[i + 1] - 32;
}
Node* search_HangHoa_by_ID(LIST l, string ID_key)//tìm kiếm trong ds bằng ID
{
	Node* k;
	for (k = l.pHead; k != NULL; k = k->pNext)
		if (ID_key == k->data.ID)
		{
			return k;
		}
	return NULL;
}
void inputHangHoa(HangHoa& h) { //thêm hàng hóa vào ds
	cout << "Enter the ID Hang Hoa: ";
	getline(cin, h.ID);
	cout << "Name: ";
	getline(cin, h.Name);
	standard_name(h.Name);
	//_strupr_s((char*)h.Name.c_str(), sizeof(h.Name));
	//toupper()
	cout << "Order status (1 - Stocking, 0 - Out of stock): "; cin >> h.Status;
	if (h.Status == true) {
		cout << "Quantity: "; cin >> h.Soluong;
	}
	else {
		h.Soluong = 0;
	}
	cout << "Unit price: "; cin >> h.DonGia;
	h.ThanhTien = h.Soluong * h.DonGia;
	cin.ignore();


}
void inputMotHangHoa(LIST& l, HangHoa& h) {//Thêm 1 hàng hóa vào danh sách
	string idTmp;
	Node* k;
	cout << "Enter the ID (ID consists of 3 characters): ";
	do {
		getline(cin, idTmp);
		_strupr_s((char*)idTmp.c_str(), sizeof(idTmp));
		k = search_HangHoa_by_ID(l, idTmp);
		if (k != NULL || strlen((char*)idTmp.c_str()) != 3)
			cout << "Re_enter the ID (ID consists of 3 characters): ";
	} while (k != NULL || strlen((char*)idTmp.c_str()) != 3);
	h.ID = idTmp;
	inputHangHoa(h);
}
void inputLIST(LIST& l) {
	HangHoa h;
	inputMotHangHoa(l, h);
	Node* p = createNode(h);
	add_last_list(l, p);
}
void writeOneHangHoaToFile(fstream& f, HangHoa h) { 
	f << "\n" << h.ID << "," << h.Name << "," << "," << h.Status << "," << h.Soluong << "," << h.HanSuDung.d << "/" << h.HanSuDung.m << "/" << h.HanSuDung.y << "," << h.DonGia << "," << h.ThanhTien;
}
void writeListToFile(fstream& f, LIST l) {
	f.open("DS_HangHoa.txt", ios::out);
	sortListByID(l);
	for (Node* k = l.pHead; k != NULL; k = k->pNext) {
		writeOneHangHoaToFile(f, k->data);
	}
	f.close();
}
void readOneHangHoaFromFile(fstream& f, HangHoa& h) {
	char ch;
	f.get(ch);
	getline(f, h.ID, ',');
	getline(f, h.Name, ',');
	f >> h.Status; f.seekg(1, 1);
	f >> h.Soluong; f.seekg(1, 1);
	f >> h.DonGia; f.seekg(1, 1);
	f >> h.ThanhTien;
	f >> h.HanSuDung.d;
	f.seekg(1, 1);
	f >> h.HanSuDung.m;
	f.seekg(1, 1);
	f >> h.HanSuDung.y;
	f.seekg(1, 1);
}
void readListFromFile(fstream& f, LIST& l) {
	HangHoa h;
	f.open("../DS_HangHoa.txt", ios::in);
	if (f.is_open()) {
	while (!f.eof()) {
		/*This code checks if the file is empty by using f.peek(). 
		If the file is empty, f.peek() will return EOF, which means that we have reached the end of the file. 
		We can then break out of the loop.*/
		if (f.peek() == std::ifstream::traits_type::eof()) {
			break;
		}
		readOneHangHoaFromFile(f, h);
		Node* p = createNode(h);
		add_last_list(l, p);
	}
	f.close();

	}
}
void outputStatus(bool Status) {
	if (Status == true)
		cout << "Stocking";
	else
		cout << "Out of stock";
}
void outputOneHangHoa(HangHoa h) {
	cout << fixed;
	cout << setprecision(2);
	cout << "| " << setw(5) << left << h.ID;
	cout << "| " << setw(25) << left << h.Name;
	cout << "| " << setw(12) << left; outputStatus(h.Status);
	cout << " | " << setfill(' ') << setw(8) << right << h.Soluong;
	cout << " | " << setfill(' ') << setw(10) << right << h.DonGia << " $";
	cout << " | " << setfill(' ') << setw(12) << right << h.ThanhTien << " $ |";
	cout << endl;
}
void outputList(LIST l) {
	if (len_list(l) == 0) {
		cout << "List empty\n";
		return;
	}
	cout << "+------+--------------------------+-----------+--------------+--------------+----------+-----------------+\n";
	cout << "| ID   | Name                     | Status    | So Luong     | Don Gia      | Thanh Tien                 |\n";
	cout << "+------+--------------------------+-----------+--------------+--------------+----------+-----------------+\n";
	for (Node* k = l.pHead; k != NULL; k = k->pNext)
		outputOneHangHoa(k->data);
	cout << "+------+--------------------------+-----------+--------------+--------------+----------+-----------------+\n";
}
void ouputHangHoaByID(LIST l) {
	string idKey; cout << "Enter your ID to search ( | ";
	for (Node* P = l.pHead; P != NULL; P = P->pNext)
		cout << P->data.ID << " | ";
	cout << "): ";
	getline(cin, idKey);
	Node* k = search_HangHoa_by_ID(l, idKey);
	if (k == NULL)
		cout << "ID does not exist\n";
	else {
		cout << "SEARCH RESULTS...\n";
		cout << "+------+--------------------------+-----------+--------------+--------------+----------+-----------------+\n";
		cout << "| ID   | Name                     | Status    | So Luong     | Don Gia      | Thanh Tien                 |\n";
		cout << "+------+--------------------------+-----------+--------------+--------------+----------+-----------------+\n";
		outputOneHangHoa(k->data);
		cout << "+------+--------------------------+-----------+--------------+--------------+----------+-----------------+\n";
	}
}


void deleteHangHoa(LIST& l, Node* P, int pos) {
	P = l.pHead;
	Node* K;
	if (pos == 1) {
		K = l.pHead;
		l.pHead = l.pHead->pNext;
		delete K;
	}
	else if (pos == len_list(l)) {
		int i = 1;
		P = l.pHead;
		while (i != pos - 1) {
			i++;
			P = P->pNext;
		}
		K = P->pNext;
		P->pNext = NULL;
		delete K;
	}
	else {
		int i = 1;
		P = l.pHead;
		while (i != pos - 1) {
			P = P->pNext;
			i++;
		}
		K = P->pNext;
		P->pNext = K->pNext;
		delete K;
	}
}
void delete_HangHoa_ID(LIST& l)
{
	string IDtemp;
	cout << "Enter ID want to delete:";
	getline(cin, IDtemp);
	Node* Q = search_HangHoa_by_ID(l, IDtemp);
	if (Q == NULL)
	{
		cout << "ID does not exist";
		return;
	}
	else
	{
		int pos = 1;
		Node* P = l.pHead;
		Node* K;
		while (P->data.ID != Q->data.ID)
		{
			pos++;
			P = P->pNext;
		}
		if (pos == 1)
		{
			K = l.pHead;
			l.pHead = l.pHead->pNext;
			delete K;
		}
		else
		{
			if (pos == len_list(l))
			{
				int i = 1;
				P = l.pHead;
				while (i != pos - 1)
				{
					i++;
					P = P->pNext;
				}

				K = Q;
				P->pNext = NULL;
				delete K;

			}
			else
			{
				int i = 1;
				P = l.pHead;
				while (i != pos - 1)
				{
					P = P->pNext;
					i++;
				}

				K = Q;
				P->pNext = K->pNext;
				delete K;
			}

		}
	}
}
void delete_list(LIST& l)
{
	Node* k = NULL;
	while (l.pHead != NULL)
	{
		k = l.pHead;
		l.pHead = l.pHead->pNext;
		delete k;
	}
}
void menu(LIST l, fstream& f) {
	//create_list(l);
	string option;
	// PLEASE CHECK IF LIST IS EMPTY OR NOT BEFORE USING THE FUNCTIONS BELOW.
	while (1) {
	
		cout << "+-------------MENU-------------+\n";
		cout << "|1. Input                      |\n";
		cout << "|2. Add a new HangHoa          |\n";
		cout << "|3. Sort HangHoa               |\n";
		cout << "|4. Delete a HangHoa           |\n";
		cout << "|5. Search a HangHoa           |\n";
		cout << "|6. Output                     |\n";
		cout << "|7. Write File                 |\n";
		cout << "|0. Exit                       |\n";
		cout << "+------------------------------+\n\n";
		cout << "Enter your option: "; getline(cin, option);
		if (option == "1") {
			cout << "Input...\n";
			inputLIST(l);
		}
		else if (option == "2") {
			// NOT WORKING, SOMETHING'S ABOUT GETLINE AND NULL-TERMINATER.
			cout << "ADD A NEW HangHoa...\n";
			inputLIST(l);
		}
		else if (option == "3") {
			// THIS IS TO CHECK IF LIST IS EMPTY. IF NOT, IT CAN SHOW MENU.
			if (l.pHead == NULL) {
				cout << "There's nothing in the list. Please try again!\n";
			}

			else {
				cout << "+--------------------------+\n";
				cout << "|1. Sort by ID             |\n";
				/*cout << "|2. Sort by Name           |\n";
				cout << "|3. Sort by Status         |\n";
				cout << "|4. Sort by SoLuong        |\n";*/
				cout << "+--------------------------+\n\n";
				string selection;

				do {
					cout << "Enter your option: ";
					getline(cin, selection);
				} while (selection <= "0" || selection > "4");
				if (selection == "1")
					sortListByID(l);
				/*else if (selection == "2")
					sortListByName(l);
				else if (selection == "3")
					sortListByStatus(l);
				else if (selection == "4")
					sortListBySoLuong(l);*/
				cout << "Sorted data successfully!!!\n";
			}
		}
		else if (option == "4") {
			cout << "+------------------------+\n";
			cout << "|1. Delete by ID         |\n";
			//cout << "|2. Delete by Name       |\n";
			cout << "+------------------------+\n\n";
			string selection;
			do {
				cout << "Enter your option: "; getline(cin, selection);
			} while (selection < "1" || selection > "2");
			if (selection == "1")
				delete_HangHoa_ID(l);
			/*else if (selection == "2")
				delete_HangHoa_Name(l);*/
			cout << "Deleted data successfully!!!\n";
		}
		else if (option == "5") {
			ouputHangHoaByID(l);
		}
		else if (option == "6") {
			outputList(l);
		}
		else if(option == "7")
		{
			writeListToFile(f, l);
			cout << "Save data successfully!!!\n";
		}
		else if (option == "0") {
			delete_list(l);
			break;
		}
		else
			continue;
	}
}
int main()
{
	LIST l; fstream f;
	create_list(l);
	readListFromFile(f, l);
	menu(l, f);
}