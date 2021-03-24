#include "linked_list.hpp"

#include <cassert>    // assert
#include <stdexcept>  // out_of_range

#include "private/internal.hpp"  // это не тот приват, о котором вы могли подумать

namespace itis {

void LinkedList::Add(Element e) {
  Node * new_node = new Node(e, nullptr);
  // Tip 1: создайте узел в куче с переданным значением
  // Tip 2: есть 2 случая - список пустой и непустой
  // Tip 3: не забудьте обновить поля head и tail
  // напишите свой код здесь ...
  if (head_ == nullptr) {
    head_ = new_node;
    tail_ = new_node;
    size_ = 1;
    return;
  }
  tail_->next = new_node;
  tail_ = new_node;
  size_ += 1;

}

void LinkedList::Insert(int index, Element e) {
  internal::check_out_of_range(index, 0, size_ + 1);
  // Tip 1: вставка элементов на позицию size эквивалентно операции добавления в конец
  // Tip 2: рассмотрите несколько случаев:
  //        (1) список пустой,
  if (head_ == nullptr){
    return;
  }
  //        (2) добавляем в начало списка,
  if (index == 0){
    Node * temp = head_;
    Node * new_node = new Node(e, temp);
    head_ = new_node;
    size_ += 1;
    return;
  }
  //        (3) добавляем в конец списка
  if (index == size_ - 1){
    Element memory = tail_->data;
    tail_->data = e;
    Add(memory);
    return;
  }
  //        (4) все остальное
  Node * curr = find_node(index - 1);
  Node * new_node = new Node(e, curr->next);
  curr->next = new_node;
  size_+= 1;

  // напишите свой код здесь ...

}

void LinkedList::Set(int index, Element e) {
  internal::check_out_of_range(index, 0, size_);
  // Tip 1: используйте функцию find_node(index)
  // напишите свой код здесь ...
  Node * curr = find_node(index);
  curr->data = e;
}

Element LinkedList::Remove(int index) {
  internal::check_out_of_range(index, 0, size_);
  // Tip 1: рассмотрите случай, когда удаляется элемент в начале списка
  // Tip 2: используйте функцию find_node(index)
  // напишите свой код здесь ...
  if (index == 0){
    Element ans = head_->data;
    Node * memory = head_->next;
    head_ = memory;
    size_ -= 1;
    return ans;
  }
  Node * curr = find_node(index);
  Element ans = curr->data;
  Node * memory = curr->next;
  Node * curr2 = find_node(index - 1);
  curr2->next = memory;
  size_ -= 1;
  return ans;
}

void LinkedList::Clear() {
  // Tip 1: люди в черном (MIB) пришли стереть вам память
  // напишите свой код здесь ...
  if (head_ == nullptr) return;
  while (head_->next != nullptr){
    Node *memory = head_->next->next;
    delete head_->next;
    head_->next = nullptr;
    size_ -= 1;
    head_->next = memory;
  }
  delete head_;
  size_ -= 1;
  head_ = nullptr;
  delete tail_;
  tail_ = nullptr;
}

Element LinkedList::Get(int index) const {
  internal::check_out_of_range(index, 0, size_);
  // напишите свой код здесь ...
  Node * curr = find_node(index);
  return curr->data;
}

int LinkedList::IndexOf(Element e) const {
  // напишите свой код здесь ...
  if (head_) {
    Node *curr = head_;
    for (int i = 0; i < size_; i++) {
      if (curr->data == e)
        return i;
      curr = curr->next;
    }
  }
  return -1;
}

Node *LinkedList::find_node(int index) const {
  assert(index >= 0 && index < size_);
  // Tip 1: можете сразу обработать случаи поиска начала и конца списка
  // напишите свой код здесь ...
  if (index == 0){
    return head_;
  }
  if (index == size_ - 1){
    return tail_;
  }
  Node *curr = head_;
  for (int i = 0; i < index; i++){
    curr = curr->next;
  }
  return curr;
}

// РЕАЛИЗОВАНО

LinkedList::~LinkedList() {
  Clear();
}

bool LinkedList::Contains(Element e) const {
  // если индекс не найден, значит и элемента нет
  return IndexOf(e) == kNotFoundElementIndex;
}

int LinkedList::GetSize() const {
  return size_;
}

bool LinkedList::IsEmpty() const {
  return size_ == 0;
}

Element LinkedList::tail() const {
  // вместо выброса ошибки в случае nullptr, римским парламентов было решено возвращать "специальное" значение
  return tail_ ? tail_->data : Element::UNINITIALIZED;
}

Element LinkedList::head() const {
  return head_ ? head_->data : Element::UNINITIALIZED;
}

// === RESTRICTED AREA: необходимо для тестирования ===

LinkedList::LinkedList(const std::vector<Element> &elements) {
  assert(!elements.empty());

  size_ = elements.size();
  head_ = new Node(elements[0], nullptr);

  auto current_node = head_;

  for (int index = 1; index < static_cast<int>(elements.size()); index++) {
    current_node->next = new Node(elements[index], nullptr);
    current_node = current_node->next;
  }
  tail_ = current_node;
}

std::ostream &operator<<(std::ostream &os, const LinkedList &list) {
  if (list.head_ != nullptr && list.tail_ != nullptr) {
    os << "{ ";
    for (auto current_node = list.head_; current_node != list.tail_; current_node = current_node->next) {
      os << internal::elem_to_str(current_node->data) << ", ";
    }
    os << internal::elem_to_str(list.tail_->data) << " }";
  } else {
    os << "{ nullptr }";
  }
  return os;
}

bool operator==(const LinkedList &list, const std::vector<Element> &elements) {
  if (list.size_ != static_cast<int>(elements.size())) return false;
  Node *current_node = list.head_;

  for (const auto e : elements) {
    if (current_node == nullptr) return false;
    if (current_node->data != e) return false;
    current_node = current_node->next;
  }
  return true;
}

}  // namespace itis
