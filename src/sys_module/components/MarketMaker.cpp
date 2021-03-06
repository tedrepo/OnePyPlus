#include "Constants.h"
#include "Environment.h"
#include "EventEngine.h"
#include "Exceptions.h"
#include "sys_module/CleanerBase.h"
#include "sys_module/ReaderBase.h"
#include "sys_module/RecorderBase.h"
#include "sys_module/components/MarketMaker.h"
#include "sys_module/models/BarBase.h"
#include "sys_module/models/Calendar.h"
#include "utils/arrow.h"
#include <functional>
#include <iostream>
#include <string>

OP_NAMESPACE_START

using namespace utils;
using std::logic_error;

MarketMaker::MarketMaker()
    : env(Environment::get_instance()),
      calendar(make_unique<Calendar>()){};

void MarketMaker::update_market() {
    try {
        env->cur_suspended_tickers.clear();
        calendar->update_calendar();
        _update_bar();
        _update_recorder();
        _check_blowup();
        env->event_engine->put(EVENT::Market_updated);

    } catch (BacktestFinished &e) {
        _update_recorder(true);
        throw BacktestFinished();
    } catch (BlowUpError &e) {
        _update_recorder(true);
        throw BacktestFinished();
    }
};

void MarketMaker::initialize() {
    std::cout << " 正在初始化OnePy" << std::endl;
    _initialize_calendar();
    _initialize_feeds();
    _initialize_cleaners();
    _check_initialize_status();
    std::cout << "OnePy初始化成功!!" << std::endl;
    std::cout << "开始寻找OnePiece之旅~~~" << std::endl;
};

void MarketMaker::_check_initialize_status() const {
    if (env->recorder == nullptr)
        throw logic_error("Recorder hasn't been not settled!");
    if (env->readers.empty())
        throw logic_error("No Readers are settled!");
}

void MarketMaker::_initialize_calendar() {
    calendar->initialize(env->instrument);
};

void MarketMaker::_initialize_feeds() {
    for (auto &value : env->readers) {
        auto ohlc_bar = get_bar(value.second->ticker, env->sys_frequency);
        if (ohlc_bar->initialize(7)) {
            const_cast<vector<string> &>(env->tickers).push_back(value.second->ticker);
            env->feeds[value.second->ticker] = ohlc_bar;
        }
    }
};

void MarketMaker::_initialize_cleaners() {
    const vector<string> tickers_copy = env->tickers;
    for (auto &ticker : tickers_copy)
        for (auto &cleaner : env->cleaners) {
            cleaner.second->initialize_buffer_data(ticker);
        }
};

void MarketMaker::_update_recorder(bool backtest_finished) {
    for (auto &recorder : env->recorders)
        recorder.second->update(backtest_finished);
};

void MarketMaker::_check_blowup() const {
    //TODO: 写逻辑
};

void MarketMaker::_update_bar() {

    for (auto &ticker : env->tickers) {
        auto bar_ptr = env->feeds[ticker];
        if (!bar_ptr->is_bar_series_end())
            bar_ptr->next();
        else {
            env->cur_suspended_tickers.push_back(ticker);
            env->suspended_tickers_record[ticker].push_back(env->sys_date);
        };
    };
};

shared_ptr<BarBase> MarketMaker::get_bar(const string &ticker,
                                         const string &frequency) {
    return env->recorder->bar_class(ticker, frequency);
};
OP_NAMESPACE_END
