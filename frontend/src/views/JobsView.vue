<template>
  <div class="page">
    <div class="container">
      <!-- Hero -->
      <div class="jobs-hero">
        <h1>Find Your Next<br><span class="gradient-text">Opportunity</span></h1>
        <!-- FIXED: Accessing data.jobs directly from the store -->
        <p>Browse {{ data.jobs.filter(j => j.isOpen).length }} open positions</p>
      </div>

      <!-- Search bar -->
      <div class="search-bar card">
        <div class="search-field">
          <svg width="18" height="18" viewBox="0 0 24 24" fill="none"><circle cx="11" cy="11" r="8" stroke="var(--c-text3)" stroke-width="1.8"/><path d="M21 21l-4.35-4.35" stroke="var(--c-text3)" stroke-width="1.8" stroke-linecap="round"/></svg>
          <input v-model="filters.query" class="search-input" placeholder="Search by title, skills, location…" />
          <button v-if="filters.query" @click="filters.query = ''" class="clear-btn">
            <svg width="14" height="14" viewBox="0 0 14 14"><path d="M2 2l10 10M12 2L2 12" stroke="currentColor" stroke-width="1.5" stroke-linecap="round"/></svg>
          </button>
        </div>
        <select v-model="filters.category" class="form-select search-select">
          <option value="">All Categories</option>
          <option v-for="cat in categories" :key="cat">{{ cat }}</option>
        </select>
        <div class="search-filters">
          <button
            class="filter-chip"
            :class="{ active: filters.remote === null }"
            @click="filters.remote = null"
          >All</button>
          <button
            class="filter-chip"
            :class="{ active: filters.remote === false }"
            @click="filters.remote = false"
          >On-Site</button>
          <button
            class="filter-chip"
            :class="{ active: filters.remote === true }"
            @click="filters.remote = true"
          >Remote</button>
        </div>
      </div>

      <!-- Sort row -->
      <div class="sort-row">
        <span class="result-count">{{ filtered.length }} results</span>
        <div class="sort-btns">
          <span class="sort-label">Sort:</span>
          <button class="sort-btn" :class="{ active: sort === 'default' }" @click="sort = 'default'">Newest</button>
          <button class="sort-btn" :class="{ active: sort === 'salary' }" @click="sort = 'salary'">Salary ↓</button>
          <button class="sort-btn" :class="{ active: sort === 'title' }" @click="sort = 'title'">Title A–Z</button>
          <!-- FIXED: Accessing auth.user directly -->
          <button v-if="auth.user?.role === 'applicant'" class="sort-btn" :class="{ active: sort === 'match' }" @click="sort = 'match'">Best Match</button>
        </div>
      </div>

      <!-- Grid -->
      <div v-if="data.loading" class="grid-2">
        <div v-for="i in 6" :key="i" class="skeleton" style="height:220px"/>
      </div>
      <div v-else-if="sorted.length === 0" class="empty-state">
        <svg width="48" height="48" viewBox="0 0 24 24" fill="none"><circle cx="11" cy="11" r="8" stroke="currentColor" stroke-width="1.5"/><path d="M21 21l-4.35-4.35" stroke="currentColor" stroke-width="1.5"/></svg>
        <p>No jobs match your search.</p>
        <button class="btn btn-outline" @click="resetFilters">Clear Filters</button>
      </div>
      <TransitionGroup v-else tag="div" class="grid-2" name="slide-up">
        <JobCard
          v-for="item in sorted"
          :key="item.job.id"
          :job="item.job"
          :userSkills="auth.user?.skills ?? []"
          :matchScore="item.matchScore"
        />
      </TransitionGroup>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, reactive } from 'vue'
import { useAuthStore } from '../stores/auth'
import { useDataStore } from '../stores/data'
import JobCard from '../components/JobCard.vue'

const auth = useAuthStore()
const data = useDataStore()

const filters = reactive({ query: '', category: '', remote: null as boolean | null })
const sort = ref<'default' | 'salary' | 'title' | 'match'>('default')

// FIXED: Using data.jobs directly strips away the Ref type confusion
const categories = computed(() => [...new Set(data.jobs.map(j => j.category))].sort())

const filtered = computed(() => data.searchJobs(filters.query, filters.category, filters.remote))

const matchScoreMap = computed(() => {
  // FIXED: Using auth.user directly strips away the Ref type confusion
  if (!auth.user) return new Map<number, number>()
  const matches = data.getMatchingJobsForUser(auth.user.skills)
  return new Map(matches.map(m => [m.job.id, m.matchScore]))
})

const sorted = computed(() => {
  const items = filtered.value.map(job => ({
    job,
    matchScore: matchScoreMap.value.get(job.id) ?? null,
  }))
  if (sort.value === 'salary') return [...items].sort((a, b) => b.job.salary - a.job.salary)
  if (sort.value === 'title') return [...items].sort((a, b) => a.job.title.localeCompare(b.job.title))
  if (sort.value === 'match') return [...items].sort((a, b) => (b.matchScore ?? 0) - (a.matchScore ?? 0))
  return items
})

function resetFilters() {
  filters.query = ''
  filters.category = ''
  filters.remote = null
}
</script>

<style scoped>
.jobs-hero {
  text-align: center;
  padding: 32px 0 40px;
}
.jobs-hero h1 { font-size: clamp(2rem, 5vw, 3.2rem); margin-bottom: 8px; }
.jobs-hero p { color: var(--c-text2); }
.gradient-text {
  background: linear-gradient(135deg, var(--c-accent), var(--c-accent2));
  -webkit-background-clip: text;
  background-clip: text;
  -webkit-text-fill-color: transparent;
}

.search-bar {
  display: flex;
  align-items: center;
  gap: 12px;
  padding: 12px 16px;
  margin-bottom: 24px;
  flex-wrap: wrap;
}
.search-field {
  display: flex;
  align-items: center;
  gap: 10px;
  flex: 1;
  min-width: 200px;
}
.search-input {
  flex: 1;
  background: none;
  border: none;
  outline: none;
  color: var(--c-text);
  font-size: .95rem;
}
.search-input::placeholder { color: var(--c-text3); }
.clear-btn {
  background: none;
  border: none;
  cursor: pointer;
  color: var(--c-text3);
  display: grid;
  place-items: center;
  padding: 2px;
}
.clear-btn:hover { color: var(--c-text2); }
.search-select { width: auto; min-width: 160px; }
.search-filters { display: flex; gap: 6px; }
.filter-chip {
  padding: 6px 14px;
  border-radius: 100px;
  border: 1px solid var(--c-border);
  background: transparent;
  color: var(--c-text2);
  font-size: .8rem;
  cursor: pointer;
  transition: all .15s;
}
.filter-chip.active, .filter-chip:hover {
  border-color: var(--c-accent);
  color: var(--c-accent);
  background: rgba(79,142,247,.08);
}

.sort-row {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: 20px;
  gap: 12px;
  flex-wrap: wrap;
}
.result-count { font-size: .85rem; color: var(--c-text3); }
.sort-btns { display: flex; align-items: center; gap: 4px; }
.sort-label { font-size: .8rem; color: var(--c-text3); margin-right: 4px; }
.sort-btn {
  padding: 5px 12px;
  border-radius: var(--radius-sm);
  border: none;
  background: transparent;
  color: var(--c-text2);
  font-size: .8rem;
  cursor: pointer;
  transition: all .15s;
}
.sort-btn.active, .sort-btn:hover { background: var(--c-surface2); color: var(--c-text); }

/* TransitionGroup */
.slide-up-move { transition: transform .3s ease; }
</style>
