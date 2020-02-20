#pragma once
template <typename T>
class listNode
{
public:
	listNode<T>* prev = nullptr;
	listNode<T>* next = nullptr;
	T* value = nullptr;
	listNode() = default;
	listNode(const T& val)
	{
		value = new T(val);
		prev = nullptr;
		next = nullptr;
	}
	~listNode()
	{
		delete value;
	}
};
template <typename T>
class list
{
	
public:
	listNode<T>* head = nullptr;
	listNode<T>* tail = nullptr;
	unsigned long len = 0;
	list() = default;
	list(const list<T> & res)
	{
		listNode<T>* tmp = res.head;
		while (tmp)
		{
			listAddNodeTail(*(tmp->value));
			tmp = tmp->next;
		}
	}
	void listAddNodeTail(const T& value)
	{
		listNode<T>* newNode = new listNode<T>(value);
		if(len==0)
		{
			head = newNode;
			tail = newNode;
			newNode->next = nullptr;
			newNode->prev = nullptr;
		}
		else
		{
			listNode<T>* tmp = tail;
			tail = newNode;
			tmp->next = newNode;
			newNode->next = nullptr;
			newNode->prev = tmp;
			
		}
		len++;
	}
	void listAddNodeHead(const T& value)
	{
		listNode<T>* newNode = new listNode<T>(value);
		if (len == 0)
		{
			head = newNode;
			tail = newNode;
		}
		else
		{
			listNode<T>* tmp = head;
			head = newNode;
			tmp->prev = newNode;
			newNode->next = tmp;
			newNode->prev = nullptr;
			
		}
		len++;
	}
	void listInsertNode(listNode<T>* old_node, const T&value, int after)
	{
		listNode<T>* newNode = new listNode<T>(value);
		if (after) {
			newNode->prev = old_node;
			newNode->next = old_node->next;
			if (tail == old_node) {
				tail = newNode;
			}
		}
		else {
			newNode->next = old_node;
			newNode->prev = old_node->prev;
			if (head == old_node) {
				head = newNode;
			}
		}
		if (newNode->prev != nullptr) {
			newNode->prev->next = newNode;
		}
		if (newNode->next != nullptr) {
			newNode->next->prev = newNode;
		}
		len++;
	}
	void listDelNode(listNode<T>* node)
	{
		if (node->prev)
			node->prev->next = node->next;
		else
			head = node->next;
		if (node->next)
			node->next->prev = node->prev;
		else
			tail = node->prev;
		delete node;
		len--;
	}
	listNode<T>* listSearchKey(T* key)
	{
		listNode<T>* tmp = head;
		while (tmp) {
			if (key == tmp->value)
				return tmp;
			if (*key == *(tmp->value)) {
				return tmp;
			}
		}
		return nullptr;
	}
	listNode<T>* listIndex(long index) {
		listNode<T>* n;

		if (index < 0) {
			index = (-index) - 1;
			n = tail;
			while (index-- && n) n = n->prev;
		}
		else {
			n = head;
			while (index-- && n) n = n->next;
		}
		return n;
	}
	~list() {
		listNode<T>* tmp = head;
		listNode<T>* next = head;
		while (tmp)
		{
			next = tmp->next;
			delete tmp;
			tmp = next;
		}
		head = nullptr;
		tail = nullptr;
		len = 0;
	}
};


