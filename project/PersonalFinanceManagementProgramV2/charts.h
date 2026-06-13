#ifndef CHARTS_H
#define CHARTS_H

/*
 * charts.h
 *
 * 本模块负责用星号显示简单图表。
 */

#include "stats.h"

void chart_category_bar(const CategoryStatsList *stats, const char *title);

#endif
