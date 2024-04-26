#pragma once

#include <complexities/complexity_analyzer.h>
#include <libds/adt/list.h>
#include <libds/adt/table.h>

namespace ds::utils
{
	template<class Table>
	class TableAnalyzer : public ComplexityAnalyzer<Table>
	{
	public:
		explicit TableAnalyzer(const std::string& name);

	protected:
		void beforeOperation(Table& table) override;
		void executeOperation(Table& table) override;

		void afterRepliciation(Table& table) override;

	private:
		void insertNElements(Table& table, size_t n);

	private:
		std::default_random_engine rngIndex_;
		std::default_random_engine rngKey_;
		adt::ImplicitList<int> keyList_;
		int key_;
	};

	class TablesAnalyzer : public CompositeAnalyzer
	{
	public:
		TablesAnalyzer() :
			CompositeAnalyzer("Tables")
		{
			this->addAnalyzer(std::make_unique<TableAnalyzer<adt::UnsortedExplicitSequenceTable<int, int>>>("uest-contains-unsorted-explicit"));
			this->addAnalyzer(std::make_unique<TableAnalyzer<adt::UnsortedImplicitSequenceTable<int, int>>>("uest-contains-unsorted-implicit"));
			//this->addAnalyzer(std::make_unique<TableAnalyzer<adt::SortedSequenceTable<int, int>>>("uest-contains"));
		}
	};

	template<class Table>
	inline TableAnalyzer<Table>::TableAnalyzer(const std::string& name) :
		ComplexityAnalyzer<Table>(name, [this](Table& table, size_t size)
			{
				this->insertNElements(table, size);
			}),
		rngIndex_(std::random_device()()),
				rngKey_(std::random_device()()),
				keyList_(),
				key_(0)
	{
	}

			template<class Table>
			inline void TableAnalyzer<Table>::beforeOperation(Table& table)
			{
				std::uniform_int_distribution<size_t> indexDist(0, this->keyList_.size() - 1);
				size_t index = indexDist(this->rngIndex_);

				this->key_ = this->keyList_.access(index);
			}

			template<class Table>
			inline void TableAnalyzer<Table>::executeOperation(Table& table)
			{

				table.contains(this->key_);
			}


			template<class Table>
			inline void TableAnalyzer<Table>::afterRepliciation(Table& table)
			{
				this->keyList_.clear();
			}

			template<class Table>
			inline void TableAnalyzer<Table>::insertNElements(Table& table, size_t n)
			{
				std::uniform_int_distribution<int> keyDist(INT_MIN, INT_MAX);


				for (size_t i = 0; i < n; i++)
				{
					int key = keyDist(this->rngKey_);
					int data = key;
					try {
						table.insert(key, data);
						this->keyList_.insertLast(key);
					}
					catch (...)
					{
						i--;
					}
				}
			}
}


