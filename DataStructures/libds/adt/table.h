#pragma once
#pragma once

#include <libds/adt/abstract_data_type.h>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/implicit_hierarchy.h>
#include <libds/amt/explicit_hierarchy.h>
#include <functional>
#include <random>
#include "libds/Heap_monitor.h"

namespace ds::adt {

	template <typename K, typename T>
	struct TableItem
	{
		K key_;
		T data_;

		bool operator==(const TableItem<K, T>& other) { return key_ == other.key_ && data_ == other.data_; }
	};

	template <typename K, typename T>
	using TabItem = TableItem<K, T>;

	//----------

	template <typename K, typename T>
	class Table :
		virtual public ADT
	{
	public:
		virtual void insert(K key, T data) = 0;
		virtual T& find(K key);
		virtual bool tryFind(K key, T*& data) = 0;
		virtual bool contains(K key);
		virtual T remove(K key) = 0;
	};

	//----------

	template <typename K, typename T, typename SequenceType>
	class SequenceTable :
		public Table<K, T>,
		public ADS<TabItem<K, T>>
	{
	public:
		SequenceTable();
		SequenceTable(const SequenceTable& other);

		bool tryFind(K key, T*& data) override;

	public:
		using BlockType = typename SequenceType::BlockType;
		using IteratorType = typename SequenceType::IteratorType;

		virtual BlockType* findBlockWithKey(K key) = 0;

		IteratorType begin();
		IteratorType end();

	protected:
		SequenceType* getSequence();
	};

	//----------

	template <typename K, typename T, typename SequenceType>
	class UnsortedSequenceTable :
		public SequenceTable<K, T, SequenceType>
	{
	protected:
		typename SequenceType::BlockType* findBlockWithKey(K key) override;
	};

	template <typename K, typename T, typename SequenceType>
	using UnsortedSTab = UnsortedSequenceTable<K, T, SequenceType>;

	//----------

	template <typename K, typename T>
	class UnsortedImplicitSequenceTable :
		public UnsortedSequenceTable<K, T, amt::IS<TabItem<K, T>>>
	{
	public:
		void insert(K key, T data) override;
		T remove(K key) override;

	private:
		using BlockType = typename amt::IS<TabItem<K, T>>::BlockType;
	};

	template <typename K, typename T>
	using UnsortedISTab = UnsortedImplicitSequenceTable<K, T>;

	//----------

	template <typename K, typename T>
	class UnsortedExplicitSequenceTable :
		public UnsortedSequenceTable<K, T, amt::SinglyLS<TabItem<K, T>>>
	{
	public:
		void insert(K key, T data) override;
		T remove(K key) override;

	private:
		using BlockType = typename amt::SinglyLS<TabItem<K, T>>::BlockType;
	};

	template <typename K, typename T>
	using UnsortedESTab = UnsortedExplicitSequenceTable<K, T>;

	//----------

	template <typename K, typename T>
	class SortedSequenceTable :
		public SequenceTable<K, T, amt::IS<TabItem<K, T>>>
	{
	public:
		void insert(K key, T data) override;
		T remove(K key) override;

	protected:
		using BlockType = typename amt::IS<TabItem<K, T>>::BlockType;

		BlockType* findBlockWithKey(K key) override;

	private:
		bool tryFindBlockWithKey(K key, size_t firstIndex, size_t lastIndex, BlockType*& lastBlock);
	};

	//----------

	template <typename K, typename T>
	using SortedSTab = SortedSequenceTable<K, T>;

	//----------

	template <typename K, typename T>
	class HashTable :
		public Table<K, T>,
		public AUMS<TabItem<K, T>>
	{
	public:
		using HashFunctionType = std::function<size_t(K)>;

	public:
		HashTable();
		HashTable(const HashTable& other);
		HashTable(HashFunctionType hashFunction, size_t capacity);
		~HashTable();

		ADT& assign(const ADT& other) override;
		bool equals(const ADT& other) override;
		void clear() override;
		size_t size() const override;
		bool isEmpty() const override;

		void insert(K key, T data) override;
		bool tryFind(K key, T*& data) override;
		T remove(K key) override;

	private:
		using SynonymTable = UnsortedESTab<K, T>;
		using SynonymTableIterator = typename SynonymTable::IteratorType;
		using PrimaryRegionIterator = typename amt::IS<SynonymTable*>::IteratorType;

	private:
		static const size_t CAPACITY = 100;

	private:
		amt::IS<SynonymTable*>* primaryRegion_;
		HashFunctionType hashFunction_;
		size_t size_;

	public:
		class HashTableIterator
		{
		public:
			HashTableIterator(PrimaryRegionIterator* tablesFirst, PrimaryRegionIterator* tablesLast);
			HashTableIterator(const HashTableIterator& other);
			~HashTableIterator();
			HashTableIterator& operator++();
			HashTableIterator operator++(int);
			bool operator==(const HashTableIterator& other) const;
			bool operator!=(const HashTableIterator& other) const;
			TabItem<K, T>& operator*();

		private:
			PrimaryRegionIterator* tablesCurrent_;
			PrimaryRegionIterator* tablesLast_;
			SynonymTableIterator* synonymIterator_;
		};

		using IteratorType = HashTableIterator;

		IteratorType begin() const;

		IteratorType end() const;
	};

	//----------

	template <typename K, typename T, typename BlockType>
	class GeneralBinarySearchTree :
		public Table<K, T>,
		public ADS<TabItem<K, T>>
	{
	public:
		using IteratorType = amt::BinaryEH<BlockType>;

	public:
		GeneralBinarySearchTree();
		GeneralBinarySearchTree(const GeneralBinarySearchTree& other);
		~GeneralBinarySearchTree();

		size_t size() const override;
		bool isEmpty() const override;

		void insert(K key, T data) override;
		bool tryFind(K key, T*& data) override;
		void clear() override;
		T remove(K key) override;

		IteratorType begin() const;
		IteratorType end() const;

	protected:
		using BVSNodeType = typename amt::BinaryEH<BlockType>::BlockType;

		amt::BinaryEH<BlockType>* getHierarchy() const;

		virtual BVSNodeType* findNodeWithRelation(K key);
		virtual BVSNodeType& insertNode(K key, BVSNodeType* relative);
		virtual void removeNode(BVSNodeType* node);
		virtual void balanceTree(BVSNodeType* node) { }

		bool tryFindNodeWithKey(K key, BVSNodeType*& node) const;

		void rotateLeft(BVSNodeType* node);
		void rotateRight(BVSNodeType* node);

	private:
		size_t size_;
	};

	//----------

	template <typename K, typename T>
	class BinarySearchTree :
		public GeneralBinarySearchTree<K, T, TabItem<K, T>>
	{
	};

	//----------

	template <typename K, typename T>
	struct TreapItem :
		public TabItem<K, T>
	{
		int priority_;
	};

	template <typename K, typename T>
	class Treap :
		public GeneralBinarySearchTree<K, T, TreapItem<K, T>>
	{
	public:
		Treap();

	protected:
		using BVSNodeType = typename GeneralBinarySearchTree<K, T, TreapItem<K, T>>::BVSNodeType;

		void removeNode(BVSNodeType* node) override;
		void balanceTree(BVSNodeType* node) override;

	private:
		std::default_random_engine rng_;
	};

	//----------

	template<typename K, typename T>
	T& Table<K, T>::find(K key)
	{
		T* data = nullptr;
		if (!this->tryFind(key, data))
		{
			throw structure_error("No such key!");
		}
		return *data;
	}

	template<typename K, typename T>
	bool Table<K, T>::contains(K key)
	{
		T* data = nullptr;
		return this->tryFind(key, data);
	}

	//----------

	template<typename K, typename T, typename SequenceType>
	SequenceTable<K, T, SequenceType>::SequenceTable() :
		ADS<TabItem<K, T>>(new SequenceType())
	{
	}

	template<typename K, typename T, typename SequenceType>
	SequenceTable<K, T, SequenceType>::SequenceTable(const SequenceTable& other) :
		ADS<TabItem<K, T>>(new SequenceType(), other)
	{
	}

	template<typename K, typename T, typename SequenceType>
	bool SequenceTable<K, T, SequenceType>::tryFind(K key, T*& data)
	{
		// TODO 10 hot
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");


		BlockType* block = this->findBlockWithKey(key);
		if (block == nullptr)
		{
			return false;
		}
		data = &block->data_.data_;
		return true;
	}

	template <typename K, typename T, typename SequenceType>
	auto SequenceTable<K, T, SequenceType>::begin() -> IteratorType
	{
		return this->getSequence()->begin();
	}

	template <typename K, typename T, typename SequenceType>
	auto SequenceTable<K, T, SequenceType>::end() -> IteratorType
	{
		return this->getSequence()->end();
	}

	template<typename K, typename T, typename SequenceType>
	SequenceType* SequenceTable<K, T, SequenceType>::getSequence()
	{
		return dynamic_cast<SequenceType*>(this->memoryStructure_);
	}

	//----------

	template<typename K, typename T, typename SequenceType>
	typename SequenceType::BlockType* UnsortedSequenceTable<K, T, SequenceType>::findBlockWithKey(K key)
	{
		// TODO 10 hot
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");

		return this->getSequence()->findBlockWithProperty([&key](typename SequenceType::BlockType* b) -> bool {
			return key == b->data_.key_;
		});

	}

	//----------

	template<typename K, typename T>
	void UnsortedImplicitSequenceTable<K, T>::insert(K key, T data)
	{
		// TODO 10 hot
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");

		if (this->contains(key))
		{
			this->error("UnsortedImplicitSequenceTable<K, T>::insert : Key is present in table");
		}
		this->getSequence()->insertLast().data_ = { key, data };
		//TableItem<K, T>* item = this->getSequence()->insertLast().data_;
		//item->key = key;
		//item->data_ = data;
	}

	template<typename K, typename T>
	T UnsortedImplicitSequenceTable<K, T>::remove(K key)
	{
		// TODO 10 hot
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");

		BlockType* block = this->findBlockWithKey(key);
		if (block == nullptr)
		{
			this->error("UnsortedImplicitSequenceTable<K, T>::remove : key is not present in table");
		}
		else
		{
			T data = block->data_.data_;
			BlockType* last = this->getSequence()->accessLast();
			if (last != block)
			{
				std::swap(block->data_, last->data_);
			}
			this->getSequence()->removeLast();

			return data;
		}

	}

	//----------

	template<typename K, typename T>
	void UnsortedExplicitSequenceTable<K, T>::insert(K key, T data)
	{
		// TODO 10 hot
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");

		if (this->contains(key))
		{
			this->error("UnsortedExplicitSequenceTable<K, T>::insert : Key is present in table");
		}

		this->getSequence()->insertLast().data_ = { key, data };

	}

	template<typename K, typename T>
	T UnsortedExplicitSequenceTable<K, T>::remove(K key)
	{
		// TODO 10 hot
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");

		BlockType* block = this->findBlockWithKey(key);
		if (block == nullptr)
		{
			this->error("UnsortedImplicitSequenceTable<K, T>::remove : key is not present in table");
		}
		else
		{
			T data = block->data_.data_;
			BlockType* first = this->getSequence()->accessFirst();
			if (first != block)
			{
				std::swap(block->data_, first->data_);
			}
			this->getSequence()->removeFirst();

			return data;
		}

	}

	//----------


	template<typename K, typename T>
	void SortedSequenceTable<K, T>::insert(K key, T data)
	{
		// TODO 10 hot
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");
		TableItem<K, T>* tableData;
		if (this->isEmpty())
		{
			tableData = &this->getSequence()->insertFirst().data_;
		}
		else
		{
			BlockType* blockWithKey = nullptr;
			if (this->tryFindBlockWithKey(key, 0, this->size(), blockWithKey))
			{
				this->error("void SortedSequenceTable<K, T>::insert(K key, T data) : key is present");
			}
			tableData = key > blockWithKey->data_.key_ ?
				&this->getSequence()->insertAfter(*blockWithKey).data_
				: &this->getSequence()->insertBefore(*blockWithKey).data_;
		}
		tableData->key_ = key;
		tableData->data_ = data;

	}

	template<typename K, typename T>
	T SortedSequenceTable<K, T>::remove(K key)
	{
		// TODO 10 hot
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");
		BlockType* blockWithKey = nullptr;

		if (!this->tryFindBlockWithKey(key, 0, this->size(), blockWithKey))
		{
			this->error("T SortedSequenceTable<K, T>::remove(K key) : item with key is not present");
		}

		T result = blockWithKey->data_.data_;
		if (this->getSequence()->accessFirst() == blockWithKey)
		{
			this->getSequence()->removeFirst();
		}
		else
		{
			this->getSequence()->removeNext(*this->getSequence()->accessPrevious(*blockWithKey));
		}
		return result;
	}

	template<typename K, typename T>
	auto SortedSequenceTable<K, T>::findBlockWithKey(K key) -> BlockType*
	{
		BlockType* blockWithKey = nullptr;
		return this->tryFindBlockWithKey(key, 0, this->size(), blockWithKey) ? blockWithKey : nullptr;
	}

	template<typename K, typename T>
	bool SortedSequenceTable<K, T>::tryFindBlockWithKey(K key, size_t firstIndex, size_t lastIndex, BlockType*& lastBlock)
	{
		// TODO 10 hot
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");

		if (this->isEmpty())
		{
			lastBlock = nullptr;
			return false;
		}

		size_t midIndex = firstIndex;
		while (firstIndex < lastIndex) {
			midIndex = firstIndex + (lastIndex - firstIndex) / 2;
			lastBlock = this->getSequence()->access(midIndex);

			if (lastBlock->data_.key_ < key)
			{
				firstIndex = midIndex + 1;
			}
			else
			{
				if (lastBlock->data_.key_ > key)
				{
					lastIndex = midIndex;
				}
				else
				{
					break;
				}
			}
		}
		lastBlock = this->getSequence()->access(midIndex);
		return lastBlock->data_.key_ == key;
	}

	//----------

	template<typename K, typename T>
	HashTable<K, T>::HashTable() :
		HashTable([](K key) { return std::hash<K>()(key); }, CAPACITY)
	{
	}

	template <typename K, typename T>
	HashTable<K, T>::HashTable(const HashTable& other) :
		primaryRegion_(new amt::IS<SynonymTable*>(other.primaryRegion_->size(), true)),
		hashFunction_(other.hashFunction_),
		size_(0)
	{
		assign(other);
	}

	template<typename K, typename T>
	HashTable<K, T>::HashTable(HashFunctionType hashFunction, size_t capacity) :
		primaryRegion_(new amt::IS<SynonymTable*>(capacity, true)),
		hashFunction_(hashFunction),
		size_(0)
	{
	}

	template <typename K, typename T>
	HashTable<K, T>::~HashTable()
	{
		this->clear();
		delete primaryRegion_;
	}

	template <typename K, typename T>
	ADT& HashTable<K, T>::assign(const ADT& other)
	{
		if (this != &other)
		{
			const HashTable& otherTable = dynamic_cast<const HashTable&>(other);
			this->clear();
			for (TabItem<K, T>& otherItem : otherTable)
			{
				this->insert(otherItem.key_, otherItem.data_);
			}
		}

		return *this;
	}

	template <typename K, typename T>
	bool HashTable<K, T>::equals(const ADT& other)
	{
		if (this == &other) { return true; }
		if (this->size() != other.size()) { return false; }

		const HashTable& otherTable = dynamic_cast<const HashTable&>(other);
		for (TabItem<K, T>& otherItem : otherTable)
		{
			T* otherData = nullptr;
			if (!this->tryFind(otherItem.key_, otherData) || *otherData != otherItem.data_)
			{
				return false;
			}
		}
		return true;
	}

	template <typename K, typename T>
	void HashTable<K, T>::clear()
	{
		size_ = 0;
		primaryRegion_->processAllBlocksForward([](typename amt::IS<SynonymTable*>::BlockType* blokSynoným)
			{
				delete blokSynoným->data_;
				blokSynoným->data_ = nullptr;
			});
	}

	template <typename K, typename T>
	size_t HashTable<K, T>::size() const
	{
		return size_;
	}

	template <typename K, typename T>
	bool HashTable<K, T>::isEmpty() const
	{
		return size() == 0;
	}

	template <typename K, typename T>
	void HashTable<K, T>::insert(K key, T data)
	{
		// TODO 11
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");
		size_t index = hashFunction_(key) % primaryRegion_->size();
		SynonymTable* synonyms = primaryRegion_->access(index)->data_;
		if (!synonyms)
		{
			synonyms = new SynonymTable();
			primaryRegion_->access(index)->data_ = synonyms;
		}
		synonyms->insert(key, data);
		size_++;
	}

	template <typename K, typename T>
	bool HashTable<K, T>::tryFind(K key, T*& data)
	{
		// TODO 11
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");
		size_t index = hashFunction_(key) % primaryRegion_->size();
		SynonymTable* synonyms = primaryRegion_->access(index)->data_;
		if (!synonyms)
		{
			return false;
		}
		return synonyms->tryFind(key, data);
	}

	template <typename K, typename T>
	T HashTable<K, T>::remove(K key)
	{
		// TODO 11
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");
		size_t index = hashFunction_(key) % primaryRegion_->size();
		SynonymTable* synonyms = primaryRegion_->access(index)->data_;
		if (!synonyms)
		{
			this->error("No such key present!");
		}
		T item = synonyms->remove(key);
		if (synonyms->isEmpty())
		{
			delete synonyms;
			primaryRegion_->access(index)->data_ = nullptr;
		}
		size_--;
		return item;
	}

	template <typename K, typename T>
	HashTable<K, T>::HashTableIterator::HashTableIterator
	(PrimaryRegionIterator* tablesFirst, PrimaryRegionIterator* tablesLast) :
		tablesCurrent_(tablesFirst),
		tablesLast_(tablesLast)
	{
		while (*tablesCurrent_ != *tablesLast_ && **tablesCurrent_ == nullptr)
		{
			++(*tablesCurrent_);
		}
		synonymIterator_ = *tablesCurrent_ != *tablesLast_
			? new SynonymTableIterator((**tablesCurrent_)->begin())
			: nullptr;
	}

	template <typename K, typename T>
	HashTable<K, T>::HashTableIterator::HashTableIterator
	(const HashTableIterator& other) :
		tablesCurrent_(other.tablesCurrent_),
		tablesLast_(other.tablesLast_),
		synonymIterator_(other.synonymIterator_)
	{
	}

	template <typename K, typename T>
	HashTable<K, T>::HashTableIterator::~HashTableIterator()
	{
		delete tablesCurrent_;
		delete tablesLast_;
		delete synonymIterator_;
	}

	template <typename K, typename T>
	auto HashTable<K, T>::HashTableIterator::operator++() -> HashTableIterator&
	{
		// TODO 11
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");
		++(*synonymIterator_);
		SynonymTableIterator oldIt = *synonymIterator_;
		if (!(oldIt != (**tablesCurrent_)->end()))
		{
			do
			{
				++(*tablesCurrent_);
			} while (*tablesCurrent_ != *tablesLast_ && **tablesCurrent_ == nullptr);
			delete synonymIterator_;
			synonymIterator_ = nullptr;
			if (*tablesCurrent_ != *tablesLast_)
			{
				synonymIterator_ = new SynonymTableIterator((**tablesCurrent_)->begin());
			}
		}
		return *this;
	}

	template <typename K, typename T>
	auto HashTable<K, T>::HashTableIterator::operator++(int) -> HashTableIterator
	{
		HashTableIterator tmp(*this);
		operator++();
		return tmp;
	}

	template <typename K, typename T>
	bool HashTable<K, T>::HashTableIterator::operator==(const HashTableIterator& other) const
	{
		return synonymIterator_ == other.synonymIterator_ ||
			(synonymIterator_ != nullptr &&
				other.synonymIterator_ != nullptr &&
				*synonymIterator_ == *(other.synonymIterator_));
	}

	template <typename K, typename T>
	bool HashTable<K, T>::HashTableIterator::operator!=(const HashTableIterator& other) const
	{
		return !(*this == other);
	}

	template <typename K, typename T>
	TabItem<K, T>& HashTable<K, T>::HashTableIterator::operator*()
	{
		return (**synonymIterator_);
	}

	//----------

	template <typename K, typename T>
	auto HashTable<K, T>::begin() const -> IteratorType
	{
		return HashTableIterator(
			new PrimaryRegionIterator(primaryRegion_->begin()),
			new PrimaryRegionIterator(primaryRegion_->end())
		);
	}

	template <typename K, typename T>
	auto HashTable<K, T>::end() const -> IteratorType
	{
		return HashTableIterator(
			new PrimaryRegionIterator(primaryRegion_->end()),
			new PrimaryRegionIterator(primaryRegion_->end())
		);
	}

	template<typename K, typename T, typename BlockType>
	GeneralBinarySearchTree<K, T, BlockType>::GeneralBinarySearchTree() :
		ADS<TabItem<K, T>>(new amt::BinaryEH<BlockType>()),
		size_(0)
	{
	}

	template<typename K, typename T, typename BlockType>
	GeneralBinarySearchTree<K, T, BlockType>::GeneralBinarySearchTree(const GeneralBinarySearchTree& other) :
		ADS<TabItem<K, T>>(new amt::BinaryEH<BlockType>(), other),
		size_(other.size_)
	{
	}

	template<typename K, typename T, typename BlockType>
	GeneralBinarySearchTree<K, T, BlockType>::~GeneralBinarySearchTree()
	{
		this->clear();
		size_ = 0;
	}



	template<typename K, typename T, typename BlockType>
	size_t GeneralBinarySearchTree<K, T, BlockType>::size() const
	{
		return size_;
	}

	template<typename K, typename T, typename BlockType>
	bool GeneralBinarySearchTree<K, T, BlockType>::isEmpty() const
	{
		return size_ == 0;
	}

	template<typename K, typename T, typename BlockType>
	void GeneralBinarySearchTree<K, T, BlockType>::insert(K key, T data)
	{
		// TODO 11
		// po implementacii vymazte vyhodenie vynimky!
		///throw std::runtime_error("Not implemented yet");

		BVSNodeType* newNode = nullptr;
		if (this->isEmpty())
		{
			newNode = &this->getHierarchy()->emplaceRoot();
		}
		else
		{
			BVSNodeType* nodeInRelation = this->findNodeWithRelation(key);
			if (nodeInRelation->data_.key_ == key)
			{
				//this->error(" void GeneralBinarySearchTree<K, T, BlockType>::insert : key is present in table");
			}
			newNode = &this->insertNode(key, nodeInRelation);
		}
		newNode->data_.key_ = key;
		newNode->data_.data_ = data;
		++this->size_;
		this->balanceTree(newNode);
	}

	template<typename K, typename T, typename BlockType>
	bool GeneralBinarySearchTree<K, T, BlockType>::tryFind(K key, T*& data)
	{
		// TODO 11 hot
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");

		BVSNodeType* node = nullptr;
		if (!this->tryFindNodeWithKey(key, node))
		{
			return false;
		}
		data = &node->data_.data_;
		return true;
	}

	template<typename K, typename T, typename BlockType>
	T GeneralBinarySearchTree<K, T, BlockType>::remove(K key)
	{
		// TODO 11 hot
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");
		BVSNodeType* node = nullptr;
		if (!this->tryFindNodeWithKey(key, node))
		{
			this->error("T GeneralBinarySearchTree<K, T, BlockType>::remove : item with key is not present in table");
		}

		T data = node->data_.data_;
		this->removeNode(node);
		--this->size_;
		return data;
	}

	template <typename K, typename T, typename BlockType>
	auto GeneralBinarySearchTree<K, T, BlockType>::begin() const -> IteratorType
	{
		return this->getHierarchy()->begin();
	}

	template <typename K, typename T, typename BlockType>
	auto GeneralBinarySearchTree<K, T, BlockType>::end() const -> IteratorType
	{
		return this->getHierarchy()->end();
	}

	template<typename K, typename T, typename BlockType>
	amt::BinaryEH<BlockType>* GeneralBinarySearchTree<K, T, BlockType>::getHierarchy() const
	{
		return dynamic_cast<amt::BinaryEH<BlockType>*>(this->memoryStructure_);
	}

	template<typename K, typename T, typename BlockType>
	auto GeneralBinarySearchTree<K, T, BlockType>::findNodeWithRelation(K key) -> BVSNodeType*
	{
		BVSNodeType* node = nullptr;
		this->tryFindNodeWithKey(key, node);
		return node;
	}

	template<typename K, typename T, typename BlockType>
	auto GeneralBinarySearchTree<K, T, BlockType>::insertNode(K key, BVSNodeType* relative) -> BVSNodeType&
	{
		return key > relative->data_.key_
			? this->getHierarchy()->insertRightSon(*relative)
			: this->getHierarchy()->insertLeftSon(*relative);
	}

	template<typename K, typename T, typename BlockType>
	void GeneralBinarySearchTree<K, T, BlockType>::removeNode(BVSNodeType* node)
	{
		// TODO 11
		// po implementacii vymazte vyhodenie vynimky!
		//throw std::runtime_error("Not implemented yet");

		const ds::amt::ExplicitHierarchyBlock<BlockType>& nodeCon = *node;
		BVSNodeType* parent = this->getHierarchy()->accessParent(*node);
		BVSNodeType* previousInOrder = node->left_;
		BVSNodeType* son = this->getHierarchy()->hasLeftSon(*node)
			? node->left_
			: node->right_;

		switch (this->getHierarchy()->degree(*node))
		{
		case 0:
			if (this->getHierarchy()->isRoot(*node))
			{
				this->getHierarchy()->clear();
			}
			else
			{
				const ds::amt::BinaryExplicitHierarchyBlock<BlockType>* par = this->getHierarchy()->accessParent(*node);
				if (node->parent_ != nullptr && this->getHierarchy()->accessLeftSon(*par) == node)
					//if (this->getHierarchy()->isLeftSon(nodeCon))
				{
					this->getHierarchy()->removeLeftSon(*parent);
				}
				else
				{
					this->getHierarchy()->removeRightSon(*parent);
				}
			}
			break;
		case 1:
			if (node->left_ == son)
			{
				this->getHierarchy()->changeLeftSon(*node, nullptr);
			}
			else
			{
				this->getHierarchy()->changeRightSon(*node, nullptr);
			}

			if (this->getHierarchy()->isRoot(*node))
			{
				this->getHierarchy()->clear();
				this->getHierarchy()->changeRoot(son);
			}
			else
			{
				if (parent->left_ == node)
				{
					this->getHierarchy()->removeLeftSon(*parent);
					this->getHierarchy()->changeLeftSon(*parent, son);
				}
				else
				{
					this->getHierarchy()->removeRightSon(*parent);
					parent->right_ = son;
					this->getHierarchy()->changeRightSon(*parent, son);
				}
			}
			break;
		case 2:
			while (this->getHierarchy()->hasRightSon(*previousInOrder))
			{
				previousInOrder = previousInOrder->right_;
			}
			std::swap(node->data_, previousInOrder->data_);
			this->removeNode(previousInOrder);
			break;
		default:
			break;
		}
	}

	template<typename K, typename T, typename BlockType>
	void GeneralBinarySearchTree<K, T, BlockType>::clear()
	{
		this->getHierarchy()->clear();
		this->memoryStructure_->clear();
		size_ = 0;
	}

	//skuska
	template<typename K, typename T, typename BlockType>
	bool GeneralBinarySearchTree<K, T, BlockType>::tryFindNodeWithKey(K key, BVSNodeType*& node) const
	{
		// TODO 11 hot
		// po implementacii vymazte vyhodenie vynimky!
	   // throw std::runtime_error("Not implemented yet");
		if (this->isEmpty())
		{
			return false;
		}
		node = this->getHierarchy()->accessRoot();
		while (node->data_.key_ != key && !this->getHierarchy()->isLeaf(*node)) {
			if (key < node->data_.key_)
			{
				if (node->left_ != nullptr)
				{
					node = node->left_;
				}
				else
				{
					return false;
				}
			}
			else
			{
				if (node->right_ != nullptr)
				{
					node = node->right_;
				}
				else
				{
					return false;
				}
			}
		}
		return node->data_.key_ == key;
	}

	template<typename K, typename T, typename BlockType>
	void GeneralBinarySearchTree<K, T, BlockType>::rotateLeft(BVSNodeType* node)
	{
		// TODO 11
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename K, typename T, typename BlockType>
	void GeneralBinarySearchTree<K, T, BlockType>::rotateRight(BVSNodeType* node)
	{
		// TODO 11
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	//----------

	template<typename K, typename T>
	Treap<K, T>::Treap() :
		rng_(std::rand())
	{
	}

	template<typename K, typename T>
	void Treap<K, T>::removeNode(BVSNodeType* node)
	{
		// TODO 11
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	template<typename K, typename T>
	void Treap<K, T>::balanceTree(BVSNodeType* node)
	{
		// TODO 11
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}
}