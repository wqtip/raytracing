#ifndef OBJECT_LIST_H
#define OBJECT_LIST_H

#include "object.h"

class object_list :public object {
public:
	std::vector<std::shared_ptr<object>> objects;
	~object_list() = default;
	object_list()
	{
		objects.resize(0);
	}
	void add(std::shared_ptr<object> o) { objects.push_back(o); }

	bool hit(ray& r, double t_min, double t_max, hit_record& rec) const override {
		double curr_t_max = t_max;
		bool hit_detected = false;
		for (std::shared_ptr<object> o : objects)
		{
			if (o->hit(r, t_min, curr_t_max, rec))
			{
				curr_t_max = rec.t;
				hit_detected = true;
			}
		}
		return hit_detected;
	}

};


#endif
