<template>
  <div class="job-card card" @click="$router.push(`/jobs/${job.id}`)">
    <!-- Header -->
    <div class="jc-header">
      <div class="jc-logo">
        {{ job.company.charAt(0).toUpperCase() }}
      </div>
      <div class="jc-meta">
        <h3 class="jc-title">{{ job.title }}</h3>
        <span class="jc-company">{{ job.company }}</span>
      </div>
      <div class="jc-badges">
        <span v-if="!job.isOpen" class="badge badge-red">Closed</span>
        <span v-if="job.isRemote" class="badge badge-blue">Remote</span>
      </div>
    </div>

    <!-- Info row -->
    <div class="jc-info">
      <span class="jc-info-item">
        <svg width="13" height="13" viewBox="0 0 24 24" fill="none"><path d="M12 2C8.13 2 5 5.13 5 9c0 5.25 7 13 7 13s7-7.75 7-13c0-3.87-3.13-7-7-7z" stroke="currentColor" stroke-width="1.8"/><circle cx="12" cy="9" r="2.5" stroke="currentColor" stroke-width="1.8"/></svg>
        {{ job.location }}
      </span>
      <span class="jc-info-item">
        <svg width="13" height="13" viewBox="0 0 24 24" fill="none"><rect x="2" y="5" width="20" height="14" rx="2" stroke="currentColor" stroke-width="1.8"/><path d="M2 10h20" stroke="currentColor" stroke-width="1.8"/></svg>
        LKR {{ salary }}
      </span>
      <span class="jc-info-item">
        <svg width="13" height="13" viewBox="0 0 24 24" fill="none"><path d="M4 6h16M4 10h10M4 14h8" stroke="currentColor" stroke-width="1.8" stroke-linecap="round"/></svg>
        {{ job.category }}
      </span>
    </div>

    <!-- Description -->
    <p class="jc-desc">{{ truncated }}</p>

    <!-- Skills -->
    <div class="jc-skills">
      <span
        v-for="skill in job.requiredSkills.slice(0, 4)"
        :key="skill"
        class="skill-tag"
        :class="{ matched: matchedSkills.includes(skill.toLowerCase()) }"
      >{{ skill }}</span>
      <span v-if="job.requiredSkills.length > 4" class="skill-tag">+{{ job.requiredSkills.length - 4 }}</span>
    </div>

    <!-- Footer -->
    <div class="jc-footer">
      <span v-if="matchScore !== null" class="match-score">
        <svg width="12" height="12" viewBox="0 0 24 24" fill="none"><path d="M12 2l3.09 6.26L22 9.27l-5 4.87 1.18 6.88L12 17.77l-6.18 3.25L7 14.14 2 9.27l6.91-1.01L12 2z" fill="var(--c-green)"/></svg>
        {{ Math.round((matchScore ?? 0) * 100) }}% match
      </span>
      <span v-else class="jc-date">Posted recently</span>
      <button class="btn btn-outline btn-sm" @click.stop="$router.push(`/jobs/${job.id}`)">View →</button>
    </div>
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import type { Job } from '../types'

const props = defineProps<{
  job: Job
  userSkills?: string[]
  matchScore?: number | null
}>()

const salary = computed(() =>
  props.job.salary.toLocaleString('en-LK', { maximumFractionDigits: 0 })
)

const truncated = computed(() =>
  props.job.description.length > 100
    ? props.job.description.slice(0, 100) + '…'
    : props.job.description
)

const matchedSkills = computed(() =>
  (props.userSkills ?? []).map(s => s.toLowerCase())
)
</script>

<style scoped>
.job-card {
  padding: 20px;
  cursor: pointer;
  display: flex;
  flex-direction: column;
  gap: 14px;
}

.jc-header {
  display: flex;
  align-items: flex-start;
  gap: 12px;
}
.jc-logo {
  width: 44px;
  height: 44px;
  border-radius: 10px;
  background: linear-gradient(135deg, rgba(79,142,247,.2), rgba(124,90,247,.2));
  border: 1px solid rgba(79,142,247,.2);
  display: grid;
  place-items: center;
  font-family: var(--font-display);
  font-size: 1.2rem;
  font-weight: 700;
  color: var(--c-accent);
  flex-shrink: 0;
}
.jc-meta { flex: 1; min-width: 0; }
.jc-title {
  font-size: 1rem;
  font-weight: 600;
  margin-bottom: 2px;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}
.jc-company { font-size: .82rem; color: var(--c-text2); }
.jc-badges { display: flex; flex-wrap: wrap; gap: 4px; }

.jc-info {
  display: flex;
  flex-wrap: wrap;
  gap: 12px;
}
.jc-info-item {
  display: flex;
  align-items: center;
  gap: 4px;
  font-size: .78rem;
  color: var(--c-text3);
}

.jc-desc {
  font-size: .85rem;
  color: var(--c-text2);
  line-height: 1.5;
}

.jc-skills {
  display: flex;
  flex-wrap: wrap;
  gap: 6px;
}
.skill-tag.matched {
  border-color: rgba(62,207,142,.35);
  background: rgba(62,207,142,.08);
  color: var(--c-green);
}

.jc-footer {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-top: auto;
}
.match-score {
  display: flex;
  align-items: center;
  gap: 4px;
  font-size: .78rem;
  color: var(--c-green);
  font-weight: 500;
}
.jc-date { font-size: .78rem; color: var(--c-text3); }
</style>